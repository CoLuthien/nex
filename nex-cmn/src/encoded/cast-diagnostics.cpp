#include "cmn/encoded/cast.hpp"

#include <spdlog/spdlog.h>

/*
 * The out-of-line half of cast.hpp's reporting. cast.hpp is a template header, so anything it
 * logs inline would drag the logging backend into every consumer of the encoded API; only these
 * definitions see spdlog.
 */
namespace nex::detail
{

void
log_quantization_ignored(std::string const& name, std::string const& type)
{
    spdlog::debug("[encoded::cast] {} carries a quantization on the real type {}; values are "
                  "already dequantized, ignoring it",
                  name,
                  type);
}

void
log_quantization_missing(std::string const& name)
{
    spdlog::warn("[encoded::cast] {} has no quantization; copying raw fixed-point values without "
                 "dequantizing",
                 name);
}

void
log_allocation_failed(std::string const& name, std::string const& message)
{
    spdlog::error("[encoded::cast] failed to allocate the destination of '{}': {}", name, message);
}

} // namespace nex::detail
