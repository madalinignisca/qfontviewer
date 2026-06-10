# Building qfontviewer

## Dependencies

- C++17 compiler (gcc or clang)
- **CMake** ≥ 3.21 and Qt 6 Widgets — on Arch this is `qt6-base` + `cmake`; on Debian
  `qt6-base-dev` + `cmake`; on Fedora `qt6-qtbase-devel` + `cmake`.
- A generator: `ninja` (recommended) or `make`.
- For the tests: Qt 6 Test (ships with `qt6-base` / `qt6-base-dev` / `qt6-qtbase-devel`).

## Build, test, install

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
sudo cmake --install build              # default prefix /usr/local
```

**Personal install, no sudo:**

```sh
cmake -B build -G Ninja -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build
cmake --install build                   # → ~/.local/bin, ~/.local/share/...
```

**Packaging** (stage into a buildroot):

```sh
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=None -DBUILD_TESTING=OFF
cmake --build build
DESTDIR="$pkgdir" cmake --install build
```

### Options

| Option | Default | Meaning |
|--------|---------|---------|
| `CMAKE_INSTALL_PREFIX` | `/usr/local` | install prefix |
| `BUILD_TESTING` | `ON` | build the QtTest unit tests (`tests/`) |

Installed files: `${prefix}/bin/qfontviewer`,
`${prefix}/share/applications/qfontviewer.desktop`,
`${prefix}/share/icons/hicolor/scalable/apps/qfontviewer.svg`.

## Code quality

The repo ships `.clang-format` and `.clang-tidy`. CI (`.github/workflows/ci.yml`)
builds on Ubuntu and Arch, runs the tests, and runs **clang-format and clang-tidy as
blocking** checks (their versions are pinned, so they're reproducible). clazy and
cppcheck run **advisory** (they can't be version-pinned the same way). Run them locally too.

CI pins **clang-format 22.1.5** and **clang-tidy 22.1.0** via the PyPI wheels
(`pipx install clang-format==22.1.5`, `pip install clang-tidy==22.1.0`) so their
checks are identical across distros — distro clang-tidy versions differ and drift.
Use a matching 22.x locally.

```sh
clang-format --dry-run --Werror src/*.cpp src/*.h tests/*.cpp
# analysis builds are configured with clang so Qt's GCC-only flags don't trip the tools:
cmake -B build-clang -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DBUILD_TESTING=OFF
clang-tidy -p build-clang src/*.cpp
clazy-standalone -p build-clang src/*.cpp
```
