

include(thirdparty/protobuf)


add_library(lnpu_nx STATIC
    onnx.proto
)

set_target_properties(lnpu_nx PROPERTIES
    POSITION_INDEPENDENT_CODE ON)   # static lib linked into SHARED lnpu_nex

target_compile_options(lnpu_nx
    PRIVATE
    ${LNPU_DEFAULT_COMPILE_OPTIONS}
)

target_link_libraries(lnpu_nx
    PUBLIC
    protobuf::libprotobuf-lite       # onnx.proto is optimize_for = LITE_RUNTIME
)

# generated onnx.pb.h lands here; expose it so consumers (lnpu_nex) can include it
target_include_directories(lnpu_nx
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

protobuf_generate(
    TARGET lnpu_nx
    LANGUAGE cpp
    GENERATE_EXTENSIONS .pb.h .pb.cc)
