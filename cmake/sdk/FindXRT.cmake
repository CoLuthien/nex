# FindXRT.cmake
#
# Locates AMD Xilinx Runtime (XRT), the host-side runtime used to drive Ryzen AI
# NPUs (XDNA/XDNA2) through xclbin artifacts and instruction sequences.
#
# Usage:
#   cmake -DOPTAI_WITH_XRT=ON [-DXRT_ROOT=<prefix>] ...
#
# Provides:
#   xrt::xrt - imported target carrying the include directory and xrt_coreutil
#
# XRT also ships pkg-config (xrt.pc) and, on some distributions, a CMake config
# package. Neither is relied on here: the packaged layout under a single prefix
# is stable across distributions, while the config package is not.

include(FindPackageHandleStandardArgs)

if(NOT XRT_ROOT)
    set(XRT_ROOT "/opt/xilinx/xrt" CACHE PATH "Path to the XRT install prefix")
endif()

# --- headers ---
# Host code includes these as <xrt/xrt_device.h>, so the include root is the
# directory *containing* xrt/. The newer ELF-based APIs (xrt::elf, xrt::module,
# xrt::ext::kernel) live under xrt/experimental/ and need no separate entry.
find_path(XRT_INCLUDE_DIR
    NAMES xrt/xrt_device.h
    HINTS "${XRT_ROOT}"
    PATH_SUFFIXES include
)

# --- library ---
# xrt_coreutil carries the whole xrt::* native C++ API; the device-specific
# shim (libxrt_driver_xdna) is loaded at runtime, not linked.
if(WIN32)
    # Untested: no Windows XRT install has been validated against this module.
    find_library(XRT_IMPLIB
        NAMES xrt_coreutil
        HINTS "${XRT_ROOT}"
        PATH_SUFFIXES lib
    )
    find_file(XRT_DLL
        NAMES xrt_coreutil.dll
        HINTS "${XRT_ROOT}"
        PATH_SUFFIXES bin
    )
    set(_xrt_required_vars XRT_INCLUDE_DIR XRT_IMPLIB XRT_DLL)
else()
    find_library(XRT_COREUTIL_LIBRARY
        NAMES xrt_coreutil
        HINTS "${XRT_ROOT}"
        PATH_SUFFIXES lib lib64
    )
    set(_xrt_required_vars XRT_INCLUDE_DIR XRT_COREUTIL_LIBRARY)
endif()

# --- standard validation ---
find_package_handle_standard_args(XRT
    REQUIRED_VARS ${_xrt_required_vars}
    FAIL_MESSAGE "Could not find XRT under '${XRT_ROOT}'. Install the XRT development package or pass -DXRT_ROOT=<prefix>."
)

# --- imported target ---
if(XRT_FOUND AND NOT TARGET xrt::xrt)
    add_library(xrt::xrt SHARED IMPORTED)

    set_target_properties(xrt::xrt PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${XRT_INCLUDE_DIR}"
        # XRT selects std::any or boost::any based on __cplusplus. MSVC needs
        # this option to report the active /std language version.
        INTERFACE_COMPILE_OPTIONS "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/Zc:__cplusplus>"
    )

    if(WIN32)
        set_target_properties(xrt::xrt PROPERTIES
            IMPORTED_LOCATION "${XRT_DLL}"
            IMPORTED_IMPLIB   "${XRT_IMPLIB}"
        )
    else()
        set_target_properties(xrt::xrt PROPERTIES
            IMPORTED_LOCATION "${XRT_COREUTIL_LIBRARY}"
        )
    endif()
endif()
