# -----------------------------------------------------------------------------
# protobuf.cmake
#
# Pulls Protocol Buffers via FetchContent and exposes it as a dependency
# (targets: protobuf::libprotobuf, protobuf::libprotobuf-lite, protobuf::protoc,
#  and the protobuf_generate() helper).
#
# Cross-compilation:
#   protoc (the .proto compiler) must run on the *host* during the build, but a
#   cross-compiled protobuf produces a protoc for the *target* that cannot run
#   on the build machine. When CMAKE_CROSSCOMPILING is TRUE we therefore:
#     1. Fetch the protobuf runtime for the TARGET (protoc build disabled), and
#     2. Build a HOST protoc out-of-tree via ExternalProject (no toolchain file),
#        then expose it as the imported protobuf::protoc target so that
#        protobuf_generate() works transparently.
#
# Override the host protoc by configuring with:
#     -DProtobuf_PROTOC_EXECUTABLE=/path/to/host/protoc
# -----------------------------------------------------------------------------

include_guard(GLOBAL)

include(FetchContent)

set(OPTAI_PROTOBUF_VERSION "v35.1" CACHE STRING
    "protobuf git tag to fetch (must ship an abseil submodule for >= v22)")

# --- build options for the protobuf we vendor ---------------------------------
set(protobuf_BUILD_TESTS         OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_EXAMPLES      OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_CONFORMANCE   OFF CACHE BOOL "" FORCE)
set(protobuf_INSTALL             OFF CACHE BOOL "" FORCE)
set(protobuf_BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)   # static runtime lib
set(protobuf_ABSL_PROVIDER    "module" CACHE STRING "" FORCE) # use bundled abseil submodule

if(MSVC)
    # Protobuf configures Abseil with the same setting. Match the toolkit CRT
    # when consuming the Windows XRT package.
    set(protobuf_MSVC_STATIC_RUNTIME ${OPTAI_WITH_XRT} CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG        ${OPTAI_PROTOBUF_VERSION}
    GIT_SUBMODULES_RECURSE ON   # pull abseil-cpp (required by protobuf >= v22)
)

if(CMAKE_CROSSCOMPILING)
    # -------------------------------------------------------------------------
    # Target build: runtime only, no protoc (it would target the device).
    # -------------------------------------------------------------------------
    set(protobuf_BUILD_PROTOC_BINARIES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(protobuf)

    # -------------------------------------------------------------------------
    # Host protoc: reuse the already-populated source, build with the host
    # compilers (no CMAKE_TOOLCHAIN_FILE) in a separate binary dir.
    # -------------------------------------------------------------------------
    if(NOT Protobuf_PROTOC_EXECUTABLE)
        include(ExternalProject)

        set(_optai_host_protoc_prefix "${CMAKE_BINARY_DIR}/host-protoc")
        set(Protobuf_PROTOC_EXECUTABLE
            "${_optai_host_protoc_prefix}/bin/protoc${CMAKE_EXECUTABLE_SUFFIX}"
            CACHE FILEPATH "Host protoc used for code generation while cross-compiling" FORCE)

        ExternalProject_Add(host_protoc
            SOURCE_DIR   "${protobuf_SOURCE_DIR}"
            SOURCE_SUBDIR .
            CMAKE_ARGS
                -DCMAKE_BUILD_TYPE=Release
                -DCMAKE_INSTALL_PREFIX=${_optai_host_protoc_prefix}
                -Dprotobuf_BUILD_TESTS=OFF
                -Dprotobuf_BUILD_EXAMPLES=OFF
                -Dprotobuf_BUILD_CONFORMANCE=OFF
                -Dprotobuf_BUILD_PROTOC_BINARIES=ON
                -Dprotobuf_INSTALL=ON
                -Dprotobuf_ABSL_PROVIDER=module
                # Deliberately NO CMAKE_TOOLCHAIN_FILE -> native host build.
            BUILD_BYPRODUCTS "${Protobuf_PROTOC_EXECUTABLE}"
            USES_TERMINAL_CONFIGURE ON
            USES_TERMINAL_BUILD     ON
            USES_TERMINAL_INSTALL   ON
        )

        # Imported protoc target that protobuf_generate() looks for, wired to
        # the host binary and depending on the ExternalProject that produces it.
        if(NOT TARGET protobuf::protoc)
            add_executable(protobuf::protoc IMPORTED GLOBAL)
            set_target_properties(protobuf::protoc PROPERTIES
                IMPORTED_LOCATION "${Protobuf_PROTOC_EXECUTABLE}")
        endif()
        add_dependencies(protobuf::protoc host_protoc)
    endif()
else()
    # Native build: protoc is produced for the host as usual.
    set(protobuf_BUILD_PROTOC_BINARIES ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(protobuf)
endif()

# Upstream leak (protobuf >= 22): generated .pb.h -> parse_context.h does an
# unqualified #include "utf8_validity.h", but protobuf only exposes that header
# to consumers via libprotobuf (PUBLIC utf8_validity). libprotobuf-lite links it
# PRIVATE, and utf8_range.cmake's include_directories() is directory-scoped to
# the protobuf tree, so lite-only consumers never see the header. Re-export it.
if(TARGET libprotobuf-lite)
    target_include_directories(libprotobuf-lite
        INTERFACE $<BUILD_INTERFACE:${protobuf_SOURCE_DIR}/third_party/utf8_range>)
endif()

# FetchContent + add_subdirectory does NOT define the protobuf_generate() helper
# (it normally ships via protobuf's find_package config module, which we bypass).
# Pull it in explicitly from the vendored source so consumers can call it.
if(NOT COMMAND protobuf_generate)
    include(${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)
endif()

# protobuf_generate() is defined by the protobuf source we just made available.
# Usage from a consuming target, e.g.:
#
#   add_library(my_proto STATIC)
#   target_link_libraries(my_proto PUBLIC protobuf::libprotobuf)
#   protobuf_generate(
#       TARGET my_proto
#       LANGUAGE cpp
#       PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/foo.proto)
