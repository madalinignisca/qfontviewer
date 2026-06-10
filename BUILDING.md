# Building qfontviewer

## Dependencies

- C++17 compiler (gcc/clang)
- `qmake6` and Qt 6 Widgets headers — on Arch this is `qt6-base`; on Debian
  `qt6-base-dev`; on Fedora `qt6-qtbase-devel`.
- `make`

## qmake6 (default)

```sh
qmake6 PREFIX=/usr/local
make -j"$(nproc)"
make install                      # honors PREFIX and INSTALL_ROOT
```

- `PREFIX` — install prefix (default `/usr/local`). Use `$HOME/.local` for a
  no-sudo personal install, or `/usr` for distro packages.
- `INSTALL_ROOT` — staging dir for packaging (`make install INSTALL_ROOT=$pkgdir`).

Installed files:

| file | location |
|------|----------|
| binary | `$PREFIX/bin/qfontviewer` |
| desktop entry | `$PREFIX/share/applications/qfontviewer.desktop` |
| icon | `$PREFIX/share/icons/hicolor/scalable/apps/qfontviewer.svg` |

## Migrating to CMake (optional)

The C++ sources need **no** changes. Drop in this `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.21)
project(qfontviewer VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(qfontviewer
    src/main.cpp
    src/mainwindow.cpp
    src/fontfilterproxy.cpp
    src/previewwidget.cpp
    src/glyphgrid.cpp)
target_link_libraries(qfontviewer PRIVATE Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS qfontviewer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES qfontviewer.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES qfontviewer.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
```

Then:

```sh
sudo pacman -S cmake ninja        # or your distro's equivalent
cmake -B build -G Ninja -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
DESTDIR="$pkgdir" cmake --install build
```

`qt_standard_project_setup()` enables automoc, so the `Q_OBJECT` classes are
handled automatically — just like qmake does today.
