cmake_minimum_required(VERSION 3.15)
project(pybind_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find the Python environment components
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

# 2. Extract the include directory path directly from pybind11
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import pybind11; print(pybind11.get_include())"
    OUTPUT_VARIABLE PYBIND11_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
)

# 3. Create a standard C++ extension module target
add_library(speed_test MODULE hello.cpp)

# 4. Bind the required python and pybind11 include headers to your target
target_include_directories(speed_test PRIVATE "${PYBIND11_INCLUDE_DIR}")
target_link_libraries(speed_test PRIVATE Python3::Module)

# 5. Tell CMake to build it as a proper python extension (.pyd) instead of a standard .dll
set_target_properties(speed_test PROPERTIES 
    PREFIX "" 
    SUFFIX ".pyd"
)