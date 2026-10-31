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
> Requirements: CMake 3.13+ and a C++20 compiler whose standard library provides `<format>` and the time zone support that `date` needs: GCC 13+, or Clang using the libstdc++ of GCC 13+.

```sh
git clone https://github.com/Netris89/core-utils
cd core-utils
cmake -B build
cmake --build build
```

The programs are written in `build/bin`, for example `./build/bin/echo hello`.

## Tests (optional)

The unit tests need [GoogleTest](https://github.com/google/googletest) and CMake 3.20+. They are not built by default.

```sh
cmake -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build
```

If GoogleTest is installed with [vcpkg](https://github.com/microsoft/vcpkg), set the `VCPKG_ROOT` environment variable and CMake uses it automatically. Otherwise, point `CMAKE_PREFIX_PATH` to the folder where GoogleTest is installed.

## Code coverage (optional)

Coverage uses `llvm-cov`, so it needs Clang. It is not enabled by default.

```sh
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -DENABLE_COVERAGE=ON
```

## Undefined behavior detection (optional)

```sh
cmake -B build -DENABLE_SANITIZERS=ON
```