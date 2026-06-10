# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

cmake_minimum_required(VERSION 3.21)
project(qfontviewer VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # for clang-tidy / clazy

set(APP_ID io.github.madalinignisca.qfontviewer)

option(BUILD_TESTING "Build the unit tests" ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup()             # enables AUTOMOC etc. (Qt >= 6.3)

# version.h (QFV_VERSION) — generated so the version string is a proper literal.
configure_file(src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h @ONLY)

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
target_include_directories(qfontviewer PRIVATE ${CMAKE_CURRENT_BINARY_DIR})  # version.h

# Internationalization: compile translations/*.ts -> .qm and embed under :/i18n/.
# Explicit lrelease + add_resources (stable across Qt 6.x) rather than the
# one-call qt_add_translations (its embedding differs on older Qt).
qt_add_lrelease(
    TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/qfontviewer_ro.ts
    QM_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/i18n
    QM_FILES_OUTPUT_VARIABLE qm_files)
qt_add_resources(qfontviewer "translations"
    PREFIX "/i18n"
    BASE ${CMAKE_CURRENT_BINARY_DIR}/i18n
    FILES ${qm_files})

include(GNUInstallDirs)
install(TARGETS qfontviewer RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${APP_ID}.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${APP_ID}.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
install(FILES data/${APP_ID}.metainfo.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
