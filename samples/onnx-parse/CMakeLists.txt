
add_executable(sample-onnx-parse
    main.cpp
    artifact.cpp
    verify.cpp
)

target_link_libraries(sample-onnx-parse
PRIVATE
    lnpu_nex
    lnpu_cmn
)

# spdlog only to turn the loader's own logging down; nothing here logs through it.
target_include_directories(sample-onnx-parse
PRIVATE
    ${NEX_THIRDPARTY_DIRECTORY}
)

target_compile_options(sample-onnx-parse
PRIVATE
    ${LNPU_DEFAULT_COMPILE_OPTIONS}
)
