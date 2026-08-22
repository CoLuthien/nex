# FindAIEBU.cmake
#
# Locates AIEBU (AIE Binary Utilities), the assembler that wraps a raw NPU
# instruction transaction blob into an ELF loadable through xrt::elf /
# xrt::module. Needed by the ELF-based submission path.
#
# Usage:
#   cmake -DOPTAI_WITH_XRT=ON [-DAIEBU_ROOT=<prefix>] ...
#
# Provides:
#   aiebu::aiebu - imported target carrying the include directory and library
#
# AIEBU builds as a submodule of XRT, so it usually lands in the same install
# prefix; XRT_ROOT is therefore the default. Distributions that ship it
# separately (or a vendored copy) need an explicit AIEBU_ROOT.

include(FindPackageHandleStandardArgs)

if(NOT AIEBU_ROOT)
    set(AIEBU_ROOT "${XRT_ROOT}" CACHE PATH "Path to the AIEBU install prefix")
endif()

# --- headers ---
# Included as <aiebu/aiebu.h>, so the include root is the directory *containing*
# aiebu/ -- same shape as XRT's.
find_path(AIEBU_INCLUDE_DIR
    NAMES aiebu/aiebu.h
    HINTS "${AIEBU_ROOT}"
    PATH_SUFFIXES include
)

# --- library ---
# Shared on Linux, static on Windows (matching how the upstream build names it).
find_library(AIEBU_LIBRARY
    NAMES aiebu aiebu_static
    HINTS "${AIEBU_ROOT}"
    PATH_SUFFIXES lib lib64
)

# --- standard validation ---
find_package_handle_standard_args(AIEBU
    REQUIRED_VARS AIEBU_INCLUDE_DIR AIEBU_LIBRARY
    # No semicolons in this string -- FPHSA parses its arguments as a list.
    FAIL_MESSAGE "Could not find AIEBU under '${AIEBU_ROOT}'. It is built as an XRT submodule, so pass -DAIEBU_ROOT=<prefix> when it lives elsewhere."
)

# --- imported target ---
# UNKNOWN so the same module works whether the found library is shared or static.
if(AIEBU_FOUND AND NOT TARGET aiebu::aiebu)
    add_library(aiebu::aiebu UNKNOWN IMPORTED)

    set_target_properties(aiebu::aiebu PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${AIEBU_INCLUDE_DIR}"
        # Release AIEBU objects in the Windows XRT package use /GL.
        INTERFACE_LINK_OPTIONS "$<$<AND:$<LINK_LANG_AND_ID:CXX,MSVC>,$<NOT:$<CONFIG:Debug>>>:/LTCG>"
        IMPORTED_LOCATION "${AIEBU_LIBRARY}"
    )
endif()
