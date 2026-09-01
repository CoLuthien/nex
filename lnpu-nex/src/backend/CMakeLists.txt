
# backend/ holds the interfaces a runtime drives a network through, and one directory per vendor
# implementing them. A vendor directory is only built when its backend is, and it reaches the
# vendor's own sources through the include path rather than by naming files across the tree.

if (LNPU_WITH_AMD_NPU)
    add_subdirectory(amd)
endif ()

# A vendor header is reached as "amd/command.hpp" -- the spelling it had before these sources moved
# under backend/ -- so it is this directory that goes on the include path, not each vendor's own.
target_include_directories(lnpu_nex PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
