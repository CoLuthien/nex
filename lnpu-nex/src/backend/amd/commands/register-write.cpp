
#include "register-write.hpp"

namespace lnpu::nex::amd::commands
{
namespace
{
constexpr command::word kOpcode  = 0; // XAIE_IO_WRITE
constexpr command::word kOpWords = 6;

constexpr command::word kColShift = 25;
constexpr command::word kRowShift = 20;
} // namespace

register_write::register_write(parameters param) //
    : m_param(param)
{
}

void
register_write::emit(npu::placement const& where, std::vector<word>& out) const
{
    out.push_back(kOpcode);
    out.push_back(0);

    // word 2: the register, addressed relative to the tile it belongs to
    out.push_back((where.location.row << kRowShift)   //
                  | (where.location.col << kColShift) //
                  | m_param.address);

    out.push_back(0);
    out.push_back(m_param.value);           // word 4
    out.push_back(kOpWords * sizeof(word)); // payload size in bytes
}

} // namespace lnpu::nex::amd::commands
