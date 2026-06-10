# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

# Test is not pulled in by the root find_package(Qt6 ... Widgets).
find_package(Qt6 REQUIRED COMPONENTS Test Widgets)

qt_add_executable(tst_qfontviewer tst_qfontviewer.cpp)
target_link_libraries(tst_qfontviewer PRIVATE qfontviewer_core Qt6::Test)

add_test(NAME qfontviewer_tests COMMAND tst_qfontviewer)
# Run headless in CI / over SSH.
set_tests_properties(qfontviewer_tests PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
