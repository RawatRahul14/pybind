## Pybind11 Setup (Windows + MSVC + venv)

### Overview
This project demonstrates how to build a Python extension module (.pyd) using C++ and pybind11 on Windows with Visual Studio Build Tools, CMake, and a Python virtual environment.

The final result is a portable:
```bash
hello.cp311-win_amd64.pyd
```
which behaves like a normal Python module.

### 1. Create Project Structure
```swift
pybind_demo/
│── hello.cpp
│── CMakeLists.txt
│── build/     (generated later)
│── script.py  (optional for testing)
```

### 2. Create & Install Python venv
```bash
python -m venv .venv
.venv\Scripts\Activate
pip install pybind11
```

### 3. Install Required Build Tools (Windows)
Install:

- MSVC v143 C++ build tools
- Windows 10 or 11 SDK
- C++ CMake tools for Windows
- Windows Universal CRT SDK

(Installed via [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/))

### 4. Source Code
`hello.cpp`
```cpp
#include <pybind11/pybind11.h>

int add(int a, int b) {
    return a + b;
}

PYBIND11_MODULE(hello, m) {
    m.doc() = "Simple pybind11 test module";
    m.def("add", &add, "Adds two integers");
}
```

`CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.15)
project(hello)

find_package(pybind11 REQUIRED)

pybind11_add_module(hello hello.cpp)
```

### 5. Build the Module
Create build folder:
```bash
mkdir build
cd build
```

> Configure CMake using MSVC (important!)
```bash
cmake -G "Visual Studio 17 2022" -Dpybind11_DIR="path_from_pybind11_cmakedir" ..
```

Example
```bash
cmake -G "Visual Studio 17 2022" -Dpybind11_DIR="C:\Users\rahul\Desktop\pybind11\venv\Lib\site-packages\pybind11\share\cmake\pybind11" ..

```

To get the path for pybind run the below code while the venv is activated.
```bash
python -m pybind11 --cmakedir
```

Build:
```bash
cmake --build . --config Release
```

Output file appears at:
```bash
build/Release/hello.cp311-win_amd64.pyd
```

Now the c++ compiled code that can be called in python is created. Now to call the function is python, there are many to do so, the one i used is to create a directory named `cpp_packages` and created a `__init__.py` file inside it.

`cpp_packages/__init__.py`
```python
import sys
from pathlib import Path

# Folder where this __init__.py is located
current_dir = Path(__file__).resolve().parent     # → pybind11/cpp_package

# Project root = parent of cpp_package
project_root = current_dir.parent                 # → pybind11/

# Build/Release folder
release_dir = project_root / "build" / "Release"

sys.path.append(str(release_dir))

import hello
```

Then you can directly call this function in your python module.

`script.py`
```python
from cpp_package import hello

print(hello.add(10, 20))
```