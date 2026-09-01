
# Programs that use the runtime the way something outside this repository would: through the
# public headers and the shared library alone. They are built with the tree, so an interface that
# stops making sense fails the build here rather than in whoever picks it up next.
add_subdirectory(onnx-parse)
