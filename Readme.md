# Pybind11 Setup (Windows + MSVC + Pip Auto-Detect + venv)

## Overview
This project demonstrates how to build a Python extension module (`.pyd`) using C++ and pybind11 on Windows with Visual Studio Build Tools, CMake, and a Python virtual environment. 

In this setup, we install pybind11 via `pip`. The `CMakeLists.txt` file is configured to **automatically detect the pybind11 path** from your active virtual environment, so you never have to manually pass long directory paths.

The final result is a self-contained library:
```bash
speed_test.cp311-win_amd64.pyd
```
which behaves exactly like a normal Python module.

## 1. Create Project Structure
```text
pybind_demo/
│── hello.cpp
│── CMakeLists.txt
│── cpp_packages/
│   └── __init__.py
│── build/         (generated later)
│── script.py      (for testing)
```

## 2. Create Python venv & Install Pybind11
Create your virtual environment, activate it, and install pybind11 so CMake can find it later:
```bash
python -m venv .venv
.venv\Scripts\Activate
pip install pybind11
```

## 3. Install Required Build Tools (Windows)
Ensure you have the following installed via [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/):
* MSVC Build Tools for x64/x86 (Latest)
* C++ CMake tools for Windows
* Windows 11 SDK 
* Windows Universal C Runtime

## 4. Source Code

`hello.cpp`
```cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

long long sum_of_squares(long long n) {
    long long total = 0;
    for (long long i = 0; i < n; i++) {
        total += i * i;
    }
    return total;
}

PYBIND11_MODULE(speed_test, m) {
    m.doc() = "C++ speed test module";
    m.def("sum_of_squares", &sum_of_squares, "Compute sum of squares from 0 to n-1");
}
```

`CMakeLists.txt` *(Updated for Auto-Detecting pip install)*
```cmake
cmake_minimum_required(VERSION 3.15)
project(pybind_demo LANGUAGES CXX)

# Force C++17 or later, which pybind11 requires
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find the Python executable in your activated venv
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# 2. Ask Python where the pip-installed pybind11 is located
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 3. Load pybind11 using the path we just found
find_package(pybind11 REQUIRED)

# 4. Define your python module (this dictates the output .pyd name)
pybind11_add_module(speed_test hello.cpp)
```

## 5. Build the Module
Make sure your virtual environment is still activated (`.venv\Scripts\Activate`). 

Create the build folder and navigate into it:
```bash
mkdir build
cd build
```

> **Configure CMake using MSVC**
```bash
cmake -G "Visual Studio 17 2022" ..
```

**Build the project:**
```bash
cmake --build . --config Release
```

The compiled C++ module will appear at:
```bash
build/Release/speed_test.cp311-win_amd64.pyd
```

## 6. Calling the C++ Module in Python

To safely route Python to your newly built `.pyd` file, update your package directory structure to append the `Release` folder to your system path.

`cpp_packages/__init__.py`
```python
import sys
from pathlib import Path

# Folder where this __init__.py is located
current_dir = Path(__file__).resolve().parent     # → pybind_demo/cpp_packages

# Project root = parent of cpp_packages
project_root = current_dir.parent                 # → pybind_demo/

# Build/Release folder containing the .pyd file
release_dir = project_root / "build" / "Release"

# Add the Release folder to python's search path
sys.path.append(str(release_dir))

# Import the module exactly as named in PYBIND11_MODULE(speed_test, m)
import speed_test
```

Now you can run your test script from the root `pybind_demo/` folder:

`script.py`
```python
from cpp_packages import speed_test

# Call the C++ function
result = speed_test.sum_of_squares(1_000_000)
print(f"Sum of squares: {result}")
```