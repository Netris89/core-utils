# Date

Smple implementation of the POSIX compliant date Unix command-line utility, written in modern C++.
It provides a basic way to display the current date and time, with support for UTC timezone via an optional flag.

## Build
> [!IMPORTANT]
> Requirements: CMake 3.10+, a C++20-compliant compiler (e.g., GCC 10+, Clang 9+).
```sh
git clone https://github.com/Netris89/date
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

```sh
./date [-option]
```

## Option
> [!NOTE]
> More details on the date command and its behavior can be found here:
> [The Open Group - date utility](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/date.html)

| Option | Description |
|--------|-------------|
| -u | Perform operations as if the TZ environment variable was set to the string "UTC0". |