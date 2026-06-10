# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

cmake_minimum_required(VERSION 3.21)
project(qfontviewer VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # for clang-tidy / clazy

option(BUILD_TESTING "Build the unit tests" ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()             # enables AUTOMOC etc. (Qt >= 6.3)

# Reusable, GUI-free-ish core so the unit tests can link the same code the app
# runs. Q_OBJECT headers are listed as sources so AUTOMOC moc's them reliably.
add_library(qfontviewer_core STATIC
    src/mainwindow.cpp      src/mainwindow.h
    src/fontfilterproxy.cpp src/fontfilterproxy.h
    src/previewwidget.cpp   src/previewwidget.h
    src/glyphgrid.cpp       src/glyphgrid.h
    src/fontmetrics.cpp     src/fontmetrics.h)
target_link_libraries(qfontviewer_core PUBLIC Qt6::Widgets)
target_include_directories(qfontviewer_core PUBLIC src)
target_compile_options(qfontviewer_core PRIVATE -Wall -Wextra)

qt_add_executable(qfontviewer src/main.cpp)
target_link_libraries(qfontviewer PRIVATE qfontviewer_core)

include(GNUInstallDirs)
install(TARGETS qfontviewer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES qfontviewer.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES qfontviewer.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
