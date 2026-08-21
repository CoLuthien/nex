
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NEX_COMPILE_OPTIONS
    #TODO
)

# MSVC-specific options
if(MSVC)
    # Use conformant preprocessor for proper __VA_ARGS__ handling
    list(APPEND OPTAI_COMPILE_OPTIONS /Zc:preprocessor /utf-8)
endif()
