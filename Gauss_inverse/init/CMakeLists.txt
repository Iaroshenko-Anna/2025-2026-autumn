add_library(init_lib STATIC matrix.cpp)

target_include_directories(init_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(init_lib PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)