from setuptools import setup, Extension, find_packages
from Cython.Build import cythonize
import numpy as np

# Define the extension module
extensions = [
    Extension(
        "gridscape.cy_gridscape",
        ["gridscape/cy_gridscape.pyx", 
         "gridscape/gridscape.c", 
         "gridscape/client.c"],
        include_dirs=["gridscape", np.get_include(), "/usr/include"],  # Added system includes
        # extra_compile_args=["-O3"],  # Optimization flag
        language="c",
        libraries=["ncurses"],  # Link against ncurses library
        define_macros=[("HAVE_NCURSES", "1")],  # Define HAVE_NCURSES macro
    ),
]

setup(
    name="gridscape",
    version="0.1.0",
    packages=find_packages(),
    ext_modules=cythonize(extensions, language_level=3),
    install_requires=["numpy", "gymnasium", "Cython"],  # Added Cython as a dependency
    package_data={
        "gridscape": ["*.h"],  # Updated to include all header files directly in gridscape
    },
    # Use this instead of requires-python in pyproject.toml
    python_requires=">=3.7",
)