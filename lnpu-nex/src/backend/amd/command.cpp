
#include "command.hpp"

#include <map>

namespace lnpu::nex::amd
{

namespace
{
std::map<npu::generation, npu::device_spec> s_device_spec{
    {npu::npu1,
     npu::device_spec{
         .major         = 0,
         .minor         = 1,
         .generation    = 3,
         .cols          = 5,
         .rows          = 6,
         .mem_tile_rows = 1,
     }},
    {npu::npu2,
     npu::device_spec{

         .major         = 0,
         .minor         = 1,
         .generation    = 4,
         .cols          = 8,
         .rows          = 6,
         .mem_tile_rows = 1,
     }},
};
}

command_list::command_list(npu::generation device_gen, std::uint32_t occupying_cols) //
    : m_generation(device_gen),                                                      //
      m_spec(s_device_spec.at(device_gen)),                                          //
      m_instructions(),                                                              //
      m_command_list(),                                                              //
      m_cols(occupying_cols),                                                        //
      m_finalized(false)
{
}

bool
command_list::record(npu::placement const& placement, std::vector<command::unique>&& commands)
{
    if (m_finalized)
    {
        return false;
    }

    for (auto&& command : commands)
    {
        if (not record(placement, std::move(command)))
        {
            return false;
        }
    }

    return true;
}

bool
command_list::record(npu::placement const& placement, command::unique command)
{
    if (m_finalized)
    {
        return false;
    }

    m_command_list.emplace_back(placement, std::move(command));
    return true;
}

bool
command_list::finalize()
{
    if (m_finalized)
    {
        return true;
    }

    m_instructions.clear();

    m_instructions.assign(4, 0);

    for (auto& [where, command] : m_command_list)
    {
        command->emit(where, m_instructions);
    }

    m_instructions[0] = m_spec.major                //
                        | (m_spec.minor << 8)       //
                        | (m_spec.generation << 16) //
                        | (m_spec.rows << 24);
    m_instructions[1] = m_cols | (m_spec.mem_tile_rows << 8);
    m_instructions[2] = m_command_list.size();
    m_instructions[3] = m_instructions.size() * sizeof(command::word);

    m_finalized = true;

    return true;
}

std::span<command::word const>
command_list::as_instructions() const
{
    if (not m_finalized)
    {
        return {};
    }

    return m_instructions;
}

} // namespace lnpu::nex::amd
