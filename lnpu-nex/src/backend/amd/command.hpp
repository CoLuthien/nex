

#pragma once

#include <xrt/xrt_hw_context.h>

#include <vector>
#include <span>
#include <cstdint>
#include <memory>

namespace lnpu::nex::amd
{

namespace npu
{

enum bd_id : std::uint32_t
{
    bd_0,
    bd_1,
    bd_2,
    bd_3,
    bd_4,
    bd_5,
    bd_6,
    bd_7,
    bd_8,
    bd_9,
    bd_10,
    bd_11,
    bd_12,
    bd_13,
    bd_14,
    bd_15
};

enum it_channel : std::uint32_t
{
    it_channel_0,
    it_channel_1
};

enum generation : std::uint32_t
{
    npu1,
    npu2
};

enum dma_direction : std::uint32_t
{
    S2MM = 0,
    MM2S = 1,
};

struct tile
{
    std::uint32_t col;
    std::uint32_t row;
};

struct placement
{
    tile          location;
    bd_id         bd;
    it_channel    channel;
    dma_direction direction;
};

struct device_spec
{
    std::uint32_t major;
    std::uint32_t minor;
    std::uint32_t generation;
    std::uint32_t cols;
    std::uint32_t rows; // maximum number of usable npu columns
    std::uint32_t mem_tile_rows;
};

} // namespace npu

class command
{
public:
    using unique = std::unique_ptr<command>;
    using word   = std::uint32_t;
    struct spec
    {
        npu::placement  where;
        command::unique cmd;
    };

    command()          = default;
    virtual ~command() = default;

    virtual void emit(npu::placement const& placement, std::vector<word>& out) const = 0;
};

class command_list
{
    npu::generation  m_generation;
    npu::device_spec m_spec;

    std::vector<command::word> m_instructions;
    std::vector<command::spec> m_command_list;

    std::uint32_t m_cols;
    bool          m_finalized;

public:
    using unique = std::unique_ptr<command_list>;

    explicit command_list(npu::generation device_gen, std::uint32_t occupying_cols);

    // todo make boolean to error codes
    bool record(npu::placement const& placement, std::vector<command::unique>&& commands);
    bool record(npu::placement const& placement, command::unique command);
    bool finalize();

    std::span<command::word const> as_instructions() const;
};

} // namespace lnpu::nex::amd
