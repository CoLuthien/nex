
#pragma once

#include "nex-api.hpp"

#include "nex/nex-types.hpp"
#include "nex/frontend/layer-schema.hpp"

#include <memory>
#include <span>
#include <string_view>
#include <variant>

namespace lnpu::nex
{
class NEX_EXPORT layer_description
{
public:
    using shared = std::shared_ptr<layer_description>;
    using unique = std::unique_ptr<layer_description>;

    layer_description()          = default;
    virtual ~layer_description() = default;

    // use in place or with pointer type
    layer_description(layer_description const&)            = delete;
    layer_description(layer_description&&)                 = delete;
    layer_description& operator=(layer_description const&) = delete;
    layer_description& operator=(layer_description&&)      = delete;

    virtual layer_schema const* schema() const = 0;
    virtual std::string_view    name() const   = 0;

    virtual std::size_t input_count() const  = 0;
    virtual std::size_t output_count() const = 0;

    virtual std::string_view input_name(std::size_t index) const  = 0;
    virtual std::string_view output_name(std::size_t index) const = 0;

    virtual value_description const* input(std::size_t index) const  = 0;
    virtual value_description const* output(std::size_t index) const = 0;

    virtual value_description const* input(std::string_view name) const  = 0;
    virtual value_description const* output(std::string_view name) const = 0;

    virtual std::size_t      parameter_count() const                 = 0;
    virtual std::string_view parameter_name(std::size_t index) const = 0;

    virtual encoded::shared parameter(std::size_t index) const     = 0;
    virtual encoded::shared parameter(std::string_view name) const = 0;

    virtual std::span<attribute const> attributes() const                          = 0;
    virtual attribute const*           find_attribute(std::string_view name) const = 0;

    template <typename T>
    T const* attribute_as(std::string_view name) const
    {
        auto* attr = find_attribute(name);
        if (nullptr == attr)
        {
            return nullptr;
        }

        return std::get_if<T>(&attr->value);
    }
};

} // namespace lnpu::nex
