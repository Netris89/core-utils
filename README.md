# Unix Utilities in C++ (POSIX-Compliant)
## Overview

This project is a C++ implementation of classic UNIX utilities, designed to adhere strictly to the POSIX standard. Each utility aims to behave as closely as possible to its traditional counterpart, ensuring compatibility, predictability, and portability across UNIX-like systems.

> [!NOTE]
> The full list of POSIX utilities can be found [here](https://pubs.opengroup.org/onlinepubs/9699919799/idx/utilities.html)

# Features

- Implementations of core utilities such as cat, cp, mv, rm, ls, and more.

- Strict adherence to POSIX return codes and error handling practices.

- Minimal external dependencies: only the C++ standard library.

- Easy to build and extend.

# Build Instructions
> [!IMPORTANT]
> Requirements: CMake 3.10+, a C++17-compliant compiler (e.g., GCC 8+, Clang 7+).
```sh
git clone https://github.com/Netris89/Utilities --recurse-submodules
cd Utilities
mkdir build
cd build
cmake ..
cmake --build .
```
>[!TIP]
> All executables will be generated inside build/bin/.
> For example, you can run ./build/bin/echo to use the echo utility.