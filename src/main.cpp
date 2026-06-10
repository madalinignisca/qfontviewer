// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

#include <QApplication>
#include <QCommandLineParser>
#include <QLocale>
#include <QTranslator>

#include "mainwindow.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("qfontviewer"));
    QApplication::setApplicationDisplayName(QStringLiteral("Qt Font Viewer"));
    QApplication::setApplicationVersion(QStringLiteral(QFV_VERSION));
    // On Wayland (Sway) the app_id the compositor sees comes from the desktop file
    // name. Must be set before any window is shown; matches the installed .desktop.
    QGuiApplication::setDesktopFileName(QStringLiteral("io.github.madalinignisca.qfontviewer"));

    // Load the embedded translation for the system locale, if present (.qm files
    // are compiled from translations/*.ts and embedded under :/i18n/). Installed
    // before parsing so --help/--version text can be localized.
    QTranslator translator;
    if (translator.load(QLocale::system(), QStringLiteral("qfontviewer"), QStringLiteral("_"),
            QStringLiteral(":/i18n"))) {
        QCoreApplication::installTranslator(&translator);
    }

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate(
        "main", "Minimal Qt6 font viewer with a console-rendering preview."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
