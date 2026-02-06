# Overview

This project is a C++ implementation of classic UNIX utilities, designed to adhere strictly to the POSIX standard. Each utility aims to behave as closely as possible to its traditional counterpart, ensuring compatibility, predictability, and portability across UNIX-like systems.  

![C++](https://img.shields.io/badge/C%2B%2B-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)![CMake](https://img.shields.io/badge/CMake-8BC34A?style=for-the-badge&logo=cmake&logoColor=white)![POSIX](https://img.shields.io/badge/POSIX-compliant-lightgrey?style=for-the-badge)![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)![Version](https://img.shields.io/badge/version-0.1-blue?style=for-the-badge)

> [!NOTE]
> The full list of POSIX utilities can be found [here](https://pubs.opengroup.org/onlinepubs/9799919799/idx/utilities.html)

# Features

- Implementations of core utilities such as cat, cp, mv, rm, ls, and more.

- Strict adherence to POSIX return codes and error handling practices.

- Minimal external dependencies: only the C++ standard library & POSIX standard library.

- Easy to build and extend.

# Build Instructions
> [!IMPORTANT]
> Requirements: CMake 3.10+, a C++20-compliant compiler (e.g., GCC 10+, Clang 9+).
```sh
git clone https://github.com/Netris89/core-utils --recurse-submodules
cd core-utils
mkdir build
cd build
cmake ..
cmake --build .
```
>[!TIP]
> All executables will be generated inside build/bin/.
> For example, you can run ./build/bin/echo to use the echo utility.