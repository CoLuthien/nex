
#pragma once

#ifdef _WIN32

#ifndef _ARM64_
#define _ARM64_ // TODO MOVE this to cmakelist
#endif

#define NOMINMAX
#define NOWINBASEINTERLOCK
#include <processenv.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

#include <string>

namespace lnpu
{

namespace env
{

#ifdef _WIN32
static constexpr auto kLdLibraryPath = "PATH";
#else
static constexpr auto kLdLibraryPath = "LD_LIBRARY_PATH";
#endif

inline void
set(std::string const& name, std::string const& value)
{
#ifdef _WIN32
    SetEnvironmentVariable(name.c_str(), value.c_str());
#else
    ::setenv(name.c_str(), value.c_str(), 1);
#endif
}

} // namespace env

} // namespace lnpu
