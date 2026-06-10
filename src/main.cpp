// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("qfontviewer");
    app.setApplicationDisplayName("Qt Font Viewer");
    // On Wayland (Sway) the app_id the compositor sees comes from the desktop
    // file name. Must be set before any window is shown. Matches qfontviewer.desktop.
    QGuiApplication::setDesktopFileName("qfontviewer");

    MainWindow w;
    w.show();
    return app.exec();
}
