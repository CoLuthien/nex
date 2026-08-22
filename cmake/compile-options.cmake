
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LNPU_DEFAULT_COMPILE_OPTIONS
    #TODO
)

# MSVC-specific options
if(MSVC)
    # Use conformant preprocessor for proper __VA_ARGS__ handling
    list(APPEND LNPU_DEFAULT_COMPILE_OPTIONS /Zc:preprocessor /utf-8)
endif()
