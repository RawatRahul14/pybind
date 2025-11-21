import sys
from pathlib import Path

# Folder where this __init__.py is located
current_dir = Path(__file__).resolve().parent     # → pybind11/cpp_package

# Project root = parent of cpp_package
project_root = current_dir.parent             

# Build/Release folder
release_dir = project_root / "build" / "Release"

sys.path.append(str(release_dir))

import speed_test