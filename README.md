# ThornLib

A small, dependency-free template for portable **C99** libraries.

ThornLib is meant to be copied, renamed, and filled in. It gives you a sane
CMake project, strict compiler warnings, sanitizer support, tests, examples,
install/export rules, `pkg-config` metadata, and CI without imposing a
framework or runtime on your code.

## Features

- **C99, no extensions** — `C_STANDARD 99`, `C_EXTENSIONS NO`.
- **No third-party dependencies** — the test harness and core headers are
  vendored and header-only.
- **Static or shared** — controlled by the standard `BUILD_SHARED_LIBS` option.
- **Correct symbol visibility** — `THORN_API` handles Windows DLL import/export
  and GCC/Clang visibility.
- **Installable** — `find_package(ThornLib)`, `Thorn::thorn`, and a
  `thorn.pc` file for `pkg-config`.
- **Strict warnings** — `-Wall -Wextra -Wpedantic` plus conversion, shadowing,
  prototype, and format checks; optional `-Werror`.
- **Sanitizers** — AddressSanitizer + UndefinedBehaviorSanitizer behind an
  option.
- **CTest** — tests are plain executables, no framework required.
- **CMake presets** — ready-made `debug`, `release`, `shared`, and `asan`
  configurations.
- **CI** — GitHub Actions matrix for GCC, Clang, macOS, Windows/MSVC, plus
  sanitizer and install/consume jobs.
- **Nix** — optional `flake.nix` dev shell and package.

## Requirements

- CMake ≥ 3.21
- A C99 compiler (GCC, Clang, or MSVC)
- Ninja or Make (optional, but recommended)

With Nix, all of this is provided by the flake:

```sh
nix develop
```

## Quick start

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/examples/thorn_example
```

Or use the bundled presets:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Shared library build:

```sh
cmake -S . -B build -DBUILD_SHARED_LIBS=ON
```

Strict mode with sanitizers:

```sh
cmake -S . -B build \
  -DTHORN_WARNINGS_AS_ERRORS=ON \
  -DTHORN_ENABLE_SANITIZERS=ON \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Layout

```
.
├── CMakeLists.txt          # project definition, options, install rules
├── CMakePresets.json       # debug / release / shared / asan presets
├── cmake/                  # warning/sanitizer helpers, package config, .pc template
├── include/thorn/          # public headers
│   ├── thorn.h             # umbrella header
│   └── core/               # one directory per module
│       ├── attributes.h    # portable compiler macros
│       ├── export.h        # THORN_API visibility macros
│       └── version.h.in    # generated from the CMake project version
├── src/core/               # implementation, mirrors include/ layout
│   └── version.c
├── tests/                  # one executable per test file
│   ├── test.h              # minimal assertion harness
│   └── core/test_version.c
├── examples/               # small programs that link the library
│   └── demo.c
├── .github/workflows/ci.yml
├── .clang-format
├── .editorconfig
└── flake.nix               # optional Nix dev shell
```

## Using this as a template

1. Copy the repository (or use it as a GitHub template).
2. Rename the project:
   - `project(ThornLib ...)` in `CMakeLists.txt`
   - `THORN_LIBRARY_TARGET` / `THORN_NAMESPACE`
   - the `thorn/` include directory and the `thorn_` symbol prefix
   - `thorn.pc.in`, `ThornLibConfig.cmake.in`, and the CI consumer test
3. Update `DESCRIPTION` and `HOMEPAGE_URL` in `CMakeLists.txt`, the README,
   and the `flake.nix` metadata.
4. Set the initial `VERSION` in `project(...)`.
5. Delete `examples/` and `tests/core/test_version.c` once you have real code.

The only place the version is written is `project(... VERSION x.y.z)`. The
generated `version.h` and the package metadata follow it automatically.

## Adding a module

A module is a pair of directories and a source list entry.

1. Add the public header:

   ```c
   /* include/thorn/core/buffer.h */
   #ifndef THORN_CORE_BUFFER_H
   #define THORN_CORE_BUFFER_H

   #include <stddef.h>

   #include "thorn/core/export.h"

   #ifdef __cplusplus
   extern "C" {
   #endif

   THORN_API int thorn_buffer_init(void);

   #ifdef __cplusplus
   }
   #endif

   #endif
   ```

2. Add the implementation in `src/core/buffer.c` and include its own header
   first.

3. List the source in `THORN_SOURCES` in `CMakeLists.txt`.

4. Add a test in `tests/core/test_buffer.c` and register it in
   `tests/CMakeLists.txt`:

   ```cmake
   add_executable(thorn_test_buffer core/test_buffer.c)
   target_link_libraries(thorn_test_buffer PRIVATE Thorn::thorn)
   target_include_directories(thorn_test_buffer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
   thorn_set_warnings(thorn_test_buffer)
   if(THORN_ENABLE_SANITIZERS)
       thorn_enable_sanitizers(thorn_test_buffer)
   endif()
   add_test(NAME core/buffer COMMAND thorn_test_buffer)
   ```

Keep implementation-private headers in `src/`; only `include/` is installed.

## Consuming the library

Include everything at once with the umbrella header, or just the modules you
need:

```c
#include "thorn/thorn.h"
```

### As a subdirectory

```cmake
add_subdirectory(third_party/ThornLib)
target_link_libraries(my_app PRIVATE Thorn::thorn)
```

### Installed package

```sh
cmake --install build --prefix /usr/local
```

```cmake
find_package(ThornLib 0.1 REQUIRED)
target_link_libraries(my_app PRIVATE Thorn::thorn)
```

### pkg-config

```sh
cc app.c $(pkg-config --cflags --libs thorn)
```

## CMake options

| Option | Default | Description |
| --- | --- | --- |
| `BUILD_SHARED_LIBS` | `OFF` | Build a shared library instead of a static one. |
| `THORN_BUILD_TESTS` | top-level only | Build the unit tests. |
| `THORN_BUILD_EXAMPLES` | top-level only | Build the examples. |
| `THORN_WARNINGS_AS_ERRORS` | `OFF` | Turn warnings into errors. |
| `THORN_ENABLE_SANITIZERS` | `OFF` | Enable ASan + UBSan (GCC/Clang). |
| `THORN_INSTALL` | top-level only | Generate install and package rules. |
| `THORN_INSTALL_PKGCONFIG` | `ON` | Install `thorn.pc`. |

## Style

- 4-space indentation, Allman braces, 100-column limit (`.clang-format`).
- Public symbols are prefixed with the project short name (`thorn_`).
- Public headers include `thorn/core/export.h` and wrap declarations in
  `extern "C"` for C++ consumers.
- Every non-`static` function has a prototype in a header (`-Wmissing-prototypes`).
- Headers use include guards and are self-contained.

## License

GPL-3.0. See [LICENSE](LICENSE).
