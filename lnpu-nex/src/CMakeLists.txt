
include(GenerateExportHeader)

add_library(lnpu_nex SHARED
    runtime.cpp
    nex-utils.cpp
)

generate_export_header(lnpu_nex
    BASE_NAME nex
    EXPORT_FILE_NAME  nex-api.hpp
    EXPORT_MACRO_NAME NEX_EXPORT
)

target_link_libraries(lnpu_nex
PUBLIC
    lnpu_cmn
)


target_include_directories(lnpu_nex
PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    $<BUILD_INTERFACE:${NEX_RUNTIME_ROOT_DIRECTORY}/include>
    $<INSTALL_INTERFACE:include>

PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${NEX_THIRDPARTY_DIRECTORY}
)

add_subdirectory(frontend/onnx)

# After the vendor directories: what lives here implements the backend interfaces on top of them,
# so it is compiled once the target it is folded into has its SDKs.
add_subdirectory(backend)


