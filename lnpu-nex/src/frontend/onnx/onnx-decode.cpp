
#include "onnx-decode.hpp"

#include "cmn/buffer/mapped-buffer.hpp"

#include <spdlog/spdlog.h>

#include <cstring>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>

namespace lnpu::nex::nx::decode
{
namespace
{

std::string
spell(std::int32_t onnx_type)
{
    auto const& name =
        ::onnx::TensorProto::DataType_Name(static_cast<::onnx::TensorProto_DataType>(onnx_type));

    return name.empty() ? "onnx type " + std::to_string(onnx_type) : name;
}

lnpu::layout
shape_of(::onnx::TensorProto const& proto)
{
    layout::shape_container extents{};
    extents.reserve(static_cast<std::size_t>(proto.dims_size()));

    for (auto const dim : proto.dims())
    {
        extents.emplace_back(static_cast<layout::extent_type>(dim));
    }

    // no dims at all is a scalar, which layout spells as rank zero and one element
    return layout{std::move(extents)};
}

/**
 * @brief Rewrites @p count elements of type Src into elements of type Dst.
 *
 * Everything is read through memcpy: raw_data is a byte string and an external weight starts
 * wherever its offset says, so neither side can be assumed aligned for the element type it holds.
 *
 * @throws std::runtime_error when an integer does not survive the conversion. The narrowing is
 *         the price of storing int64 indices as i32 (see as_encoding()), and a shape that no
 *         longer fits is worth stopping for rather than wrapping around.
 */
template <typename Src, typename Dst>
void
transcode(std::byte const* source, std::size_t count, std::byte* target, std::string const& name)
{
    if (count == 0)
    {
        return;
    }

    if constexpr (std::is_same_v<Src, Dst>)
    {
        std::memcpy(target, source, count * sizeof(Dst));
        return;
    }
    else
    {
        for (std::size_t index = 0; index < count; ++index)
        {
            Src value{};
            std::memcpy(&value, source + index * sizeof(Src), sizeof(Src));

            if constexpr (std::is_integral_v<Src> and std::is_integral_v<Dst>)
            {
                if (not std::in_range<Dst>(value))
                {
                    throw std::runtime_error("[nx::decode] '" + name + "' holds " +
                                             std::to_string(value) + " at index " +
                                             std::to_string(index) +
                                             ", which does not fit the encoding it is read into");
                }
            }

            auto const converted = static_cast<Dst>(value);
            std::memcpy(target + index * sizeof(Dst), &converted, sizeof(Dst));
        }
    }
}

/**
 * @brief The bytes of an external weight, as a window onto the file it lives in.
 *
 * @throws std::runtime_error when the file was not handed to the loader, or the window it names
 *         reaches past the end of that file.
 */
buffer::shared
external_window(::onnx::TensorProto const& proto, external_files const& externals)
{
    std::string location{};
    std::size_t offset = 0;
    std::size_t length = 0;
    bool        sized  = false;

    for (auto const& entry : proto.external_data())
    {
        if (entry.key() == "location")
        {
            location = entry.value();
        }
        else if (entry.key() == "offset")
        {
            offset = std::stoull(entry.value());
        }
        else if (entry.key() == "length")
        {
            length = std::stoull(entry.value());
            sized  = true;
        }
    }

    auto const found = externals.find(location);
    if (found == externals.end())
    {
        throw std::runtime_error("[nx::decode] '" + proto.name() + "' keeps its data in '" +
                                 location +
                                 "', which was not among the files handed to the "
                                 "loader");
    }

    auto const& file = found->second;

    if (not sized)
    {
        // no length: the tensor owns everything from its offset to the end of the file
        length = file->size_bytes() - std::min(offset, file->size_bytes());
    }

    if (offset + length > file->size_bytes())
    {
        throw std::runtime_error("[nx::decode] '" + proto.name() + "' reaches past the end of '" +
                                 location + "'");
    }

    return file->make_view(offset, length);
}

/**
 * @brief Where one tensor keeps its payload.
 *
 * `bytes` is meaningful only when `data` is set; a null `data` means the payload sits in one of
 * the typed repeated fields instead, which the caller picks by element type.
 */
struct location
{
    buffer::shared   window{};      // set when the payload came from an external file
    std::byte const* data{nullptr}; // raw_data, or the window above
    std::size_t      bytes{0};
};

location
locate(::onnx::TensorProto const& proto, external_files const& externals)
{
    if (proto.data_location() == ::onnx::TensorProto_DataLocation_EXTERNAL)
    {
        auto window = external_window(proto, externals);
        return {window, window->ptr(), window->size_bytes()};
    }

    if (proto.has_raw_data())
    {
        auto const& raw = proto.raw_data();
        return {nullptr, reinterpret_cast<std::byte const*>(raw.data()), raw.size()};
    }

    return {};
}

/**
 * @brief Fills @p target with @p count elements, from raw bytes of type Raw or, failing that,
 *        from the repeated @p field of type Field.
 *
 * The two source types differ because onnx packs the small element types into int32_data and the
 * unsigned ones into uint64_data, while raw_data always holds their own representation.
 */
template <typename Raw, typename Field, typename Dst, typename Repeated>
void
ingest(location const&    source,
       Repeated const&    field,
       std::byte*         target,
       std::size_t        count,
       std::string const& name)
{
    if (source.data != nullptr)
    {
        if (source.bytes != count * sizeof(Raw))
        {
            throw std::runtime_error("[nx::decode] '" + name + "' carries " +
                                     std::to_string(source.bytes) +
                                     " bytes where its shape asks "
                                     "for " +
                                     std::to_string(count * sizeof(Raw)));
        }

        transcode<Raw, Dst>(source.data, count, target, name);
        return;
    }

    if (static_cast<std::size_t>(field.size()) != count)
    {
        throw std::runtime_error("[nx::decode] '" + name + "' lists " +
                                 std::to_string(field.size()) +
                                 " values where its shape asks for " + std::to_string(count));
    }

    transcode<Field, Dst>(reinterpret_cast<std::byte const*>(field.data()), count, target, name);
}

void
write_payload(::onnx::TensorProto const& proto,
              location const&            source,
              std::byte*                 target,
              std::size_t                count)
{
    auto const& name = proto.name();

    switch (proto.data_type())
    {
    case ::onnx::TensorProto_DataType_FLOAT:
        return ingest<float, float, float>(source, proto.float_data(), target, count, name);
    case ::onnx::TensorProto_DataType_DOUBLE:
        return ingest<double, double, float>(source, proto.double_data(), target, count, name);

    // f16 and bf16 travel as their own bit pattern, widened into int32_data when not raw
    case ::onnx::TensorProto_DataType_FLOAT16:
    case ::onnx::TensorProto_DataType_BFLOAT16:
        return ingest<std::uint16_t, std::int32_t, std::uint16_t>(
            source, proto.int32_data(), target, count, name);

    case ::onnx::TensorProto_DataType_INT8:
        return ingest<std::int8_t, std::int32_t, std::int8_t>(
            source, proto.int32_data(), target, count, name);
    case ::onnx::TensorProto_DataType_UINT8:
    case ::onnx::TensorProto_DataType_BOOL:
        return ingest<std::uint8_t, std::int32_t, std::uint8_t>(
            source, proto.int32_data(), target, count, name);
    case ::onnx::TensorProto_DataType_INT16:
        return ingest<std::int16_t, std::int32_t, std::int16_t>(
            source, proto.int32_data(), target, count, name);
    case ::onnx::TensorProto_DataType_UINT16:
        return ingest<std::uint16_t, std::int32_t, std::uint16_t>(
            source, proto.int32_data(), target, count, name);
    case ::onnx::TensorProto_DataType_INT32:
        return ingest<std::int32_t, std::int32_t, std::int32_t>(
            source, proto.int32_data(), target, count, name);

    // the encodings that do not exist here: kept only as long as every value fits i32
    case ::onnx::TensorProto_DataType_INT64:
        return ingest<std::int64_t, std::int64_t, std::int32_t>(
            source, proto.int64_data(), target, count, name);
    case ::onnx::TensorProto_DataType_UINT32:
    case ::onnx::TensorProto_DataType_UINT64:
        return ingest<std::uint64_t, std::uint64_t, std::int32_t>(
            source, proto.uint64_data(), target, count, name);

    default:
        break;
    }

    // as_encoding() runs first and rejects everything else, so getting here is a bug in this file
    throw std::logic_error("[nx::decode] no payload reader for " + spell(proto.data_type()));
}

buffer::shared
allocate(layout const& shape, EDataType encoding, std::string const& name)
{
    std::error_code failure{};

    // An empty tensor is legal onnx but there is no zero-length mapping to hand back, so it gets
    // the smallest one there is; the layout keeps saying the region holds nothing.
    auto storage = shape.element_count() == 0 ? allocate_mapped(layout{1}, EDataType::u8, failure)
                                              : allocate_mapped(shape, encoding, failure);

    if (failure or not storage)
    {
        throw std::runtime_error("[nx::decode] could not allocate the storage of '" + name +
                                 "': " + failure.message());
    }

    return storage;
}

/**
 * @brief Says so when a per-channel quantization does not describe the axis it names.
 *
 * Only worth a warning: the parameters are still readable, and encoded itself refuses to narrow
 * them along an axis it cannot line them up with. Whether the export or the model is the odd one
 * out is not something this loader can tell.
 */
void
expect_channels(encoded const& weight)
{
    auto const& described = weight.fmt().quant;
    if (not described or not described->axis)
    {
        return;
    }

    auto const axis = *described->axis;
    if (axis >= weight.rank())
    {
        spdlog::warn("[nx::decode] '{}' is quantized along axis {} but has rank {}",
                     weight.fmt().name,
                     axis,
                     weight.rank());
        return;
    }

    auto const channels = static_cast<std::size_t>(weight.shape(axis));
    if (described->scale->n_element() != channels)
    {
        spdlog::warn("[nx::decode] '{}' carries {} scale(s) for the {} channel(s) of axis {}",
                     weight.fmt().name,
                     described->scale->n_element(),
                     channels,
                     axis);
    }
}

} // namespace

EDataType
as_encoding(std::int32_t onnx_type)
{
    switch (static_cast<::onnx::TensorProto_DataType>(onnx_type))
    {
    case ::onnx::TensorProto_DataType_FLOAT:
    case ::onnx::TensorProto_DataType_DOUBLE:
        return EDataType::f32;
    case ::onnx::TensorProto_DataType_FLOAT16:
        return EDataType::f16;
    case ::onnx::TensorProto_DataType_BFLOAT16:
        return EDataType::bf16;

    case ::onnx::TensorProto_DataType_INT8:
        return EDataType::i8;
    case ::onnx::TensorProto_DataType_UINT8:
    case ::onnx::TensorProto_DataType_BOOL:
        return EDataType::u8;
    case ::onnx::TensorProto_DataType_INT16:
        return EDataType::i16;
    case ::onnx::TensorProto_DataType_UINT16:
        return EDataType::u16;

    case ::onnx::TensorProto_DataType_INT32:
    case ::onnx::TensorProto_DataType_UINT32:
    case ::onnx::TensorProto_DataType_INT64:
    case ::onnx::TensorProto_DataType_UINT64:
        return EDataType::i32;

    default:
        break;
    }

    throw std::logic_error("[nx::decode] " + spell(onnx_type) + " has no encoding in this runtime");
}

bool
is_transcoded(std::int32_t onnx_type)
{
    switch (static_cast<::onnx::TensorProto_DataType>(onnx_type))
    {
    case ::onnx::TensorProto_DataType_DOUBLE:
    case ::onnx::TensorProto_DataType_UINT32:
    case ::onnx::TensorProto_DataType_INT64:
    case ::onnx::TensorProto_DataType_UINT64:
        return true;
    default:
        return false;
    }
}

encoded::shared
tensor(::onnx::TensorProto const&  proto,
       external_files const&       externals,
       std::optional<quantization> quant)
{
    auto const encoding = as_encoding(proto.data_type());
    auto       shape    = shape_of(proto);
    auto const count    = shape.element_count();

    auto const source = locate(proto, externals);

    buffer::shared storage{};

    if (source.window and count != 0 and not is_transcoded(proto.data_type()))
    {
        // the mapped bytes are already what we would have written, so the tensor reads them
        // where they lie instead of owning a copy
        auto const expected = count * in_bytes(encoding);
        if (source.bytes != expected)
        {
            throw std::runtime_error("[nx::decode] the external data of '" + proto.name() +
                                     "' is " + std::to_string(source.bytes) +
                                     " bytes where its shape asks for " + std::to_string(expected));
        }

        storage = source.window;
    }
    else
    {
        storage = allocate(shape, encoding, proto.name());
        write_payload(proto, source, storage->ptr(), count);
    }

    encoded::format description{
        .name     = proto.name(),
        .encoding = encoding,
        .layout   = std::move(shape),
        .quant    = std::move(quant),
    };

    return encoded::create(std::move(description), std::move(storage));
}

tensor_map
initializers(repeated<::onnx::TensorProto> const& protos,
             external_files const&                externals,
             quantization_map const&              encodings)
{
    tensor_map weights{};
    weights.reserve(static_cast<std::size_t>(protos.size()));

    for (auto const& proto : protos)
    {
        auto const described = encodings.find(proto.name());

        auto weight =
            tensor(proto,
                   externals,
                   described != encodings.end() ? std::optional<quantization>{described->second}
                                                : std::nullopt);

        expect_channels(*weight);

        weights.insert_or_assign(proto.name(), std::move(weight));
    }

    return weights;
}

std::vector<value_description>
values(repeated<::onnx::ValueInfoProto> const& protos, quantization_map const& encodings)
{
    std::vector<value_description> described{};
    described.reserve(static_cast<std::size_t>(protos.size()));

    for (auto const& proto : protos)
    {
        if (not proto.type().has_tensor_type())
        {
            spdlog::error("[nx::decode] '{}' is not a tensor; sequence, map and optional values "
                          "have no description here",
                          proto.name());
            throw std::logic_error("[nx::decode] '" + proto.name() + "' is not a tensor");
        }

        auto const& described_type = proto.type().tensor_type();

        value_description description{.name = proto.name()};

        if (described_type.has_elem_type())
        {
            description.dtype = as_encoding(described_type.elem_type());
        }

        if (described_type.has_shape())
        {
            layout::shape_container extents{};
            extents.reserve(static_cast<std::size_t>(described_type.shape().dim_size()));

            for (auto const& dim : described_type.shape().dim())
            {
                // a symbolic dimension ("batch") is not a size yet; a negative extent is how
                // layout carries one until shape inference settles it
                extents.emplace_back(dim.has_dim_value()
                                         ? static_cast<layout::extent_type>(dim.dim_value())
                                         : layout::extent_type{-1});
            }

            description.layout = layout{std::move(extents)};
        }

        if (auto const described = encodings.find(proto.name()); described != encodings.end())
        {
            description.quant = described->second;
        }

        described.emplace_back(std::move(description));
    }

    return described;
}

std::vector<attribute>
attributes(repeated<::onnx::AttributeProto> const& protos)
{
    static external_files const none{};

    std::vector<attribute> read{};
    read.reserve(static_cast<std::size_t>(protos.size()));

    for (auto const& proto : protos)
    {
        attribute parsed{.name = proto.name()};

        switch (proto.type())
        {
        case ::onnx::AttributeProto_AttributeType_FLOAT:
            parsed.value = proto.f();
            break;
        case ::onnx::AttributeProto_AttributeType_INT:
            parsed.value = static_cast<std::int64_t>(proto.i());
            break;
        case ::onnx::AttributeProto_AttributeType_STRING:
            parsed.value = proto.s();
            break;
        case ::onnx::AttributeProto_AttributeType_TENSOR:
            // an attribute tensor is inline by definition, so it needs no external file
            parsed.value = tensor(proto.t(), none);
            break;
        case ::onnx::AttributeProto_AttributeType_FLOATS:
            parsed.value = std::vector<float>{proto.floats().begin(), proto.floats().end()};
            break;
        case ::onnx::AttributeProto_AttributeType_INTS:
            parsed.value = std::vector<std::int64_t>{proto.ints().begin(), proto.ints().end()};
            break;
        case ::onnx::AttributeProto_AttributeType_STRINGS:
            parsed.value = std::vector<std::string>{proto.strings().begin(), proto.strings().end()};
            break;
        default:
            throw std::logic_error("[nx::decode] attribute '" + proto.name() + "' is of type " +
                                   ::onnx::AttributeProto::AttributeType_Name(proto.type()) +
                                   ", which has no counterpart here");
        }

        read.emplace_back(std::move(parsed));
    }

    return read;
}

} // namespace lnpu::nex::nx::decode
