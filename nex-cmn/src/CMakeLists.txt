
# --- [Library Definition] ---
add_library(nex_cmn SHARED
    layout.cpp

    buffer/buffer-view.cpp
    buffer/linux-mapped.cpp

    encoded/encoded.cpp
    encoded/encoded-slice.cpp
    encoded/encoded-print.cpp
    encoded/cast-diagnostics.cpp
    encoded/quantization.cpp
    encoded/strided-copy.cpp
)

# Export Header 생성
generate_export_header(nex_cmn
    BASE_NAME optai
    EXPORT_FILE_NAME cmn-api.hpp
    EXPORT_MACRO_NAME CMN_EXPORT
)

# --- [Linking Dependencies] ---
target_link_libraries(nex_cmn
    PUBLIC

)

# --- [Include Directories] ---
target_include_directories(nex_cmn
PUBLIC
    $<BUILD_INTERFACE:${NEX_CMN_ROOT_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_BINARY_DIR}  # for cmn-api.hpp

PRIVATE
    ${NEX_CMN_ROOT_DIR}/include/cmn

    ${CMAKE_CURRENT_SOURCE_DIR}
    ${NEX_THIRDPARTY_DIRECTORY}
)

# --- [Compile Options] ---
target_compile_options(nex_cmn
PUBLIC
    ${OPTAI_COMMON_COMPILE_OPTIONS}
)

# --- [Install] ---
install(TARGETS nex_cmn
    EXPORT nex-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin  # Windows DLL을 위해 필요
)

install(DIRECTORY ${NEX_CMN_ROOT_DIR}/include/ DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmn-api.hpp DESTINATION include)
