add_library(solver_lib STATIC gauss_inverse.cpp)

target_include_directories(solver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Указываем стандарт C++ для этой библиотеки
set_target_properties(solver_lib PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)