
add_subdirectory(proto)


target_link_libraries(lnpu_nex
PRIVATE
    lnpu_nx
)

target_sources(lnpu_nex

PRIVATE
    layer-description.cpp
    network-description.cpp
    onnx-decode.cpp
    quantization-encoding.cpp
    schema-registry.cpp

    # written by generate-op-schemas.py out of the onnx package; see op-schemas-coverage.md
    op-schemas.gen.cpp
)
