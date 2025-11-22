add_library(print_lib STATIC print.cpp)

target_include_directories(print_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(print_lib PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)