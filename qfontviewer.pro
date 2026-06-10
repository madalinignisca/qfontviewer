QT       += widgets
CONFIG   += c++17 release warn_on
TARGET    = qfontviewer
TEMPLATE  = app
VERSION   = 1.0.0

SOURCES  += src/main.cpp \
            src/mainwindow.cpp \
            src/fontfilterproxy.cpp \
            src/previewwidget.cpp \
            src/glyphgrid.cpp

HEADERS  += src/mainwindow.h \
            src/fontfilterproxy.h \
            src/previewwidget.h \
            src/glyphgrid.h

# --- Install prefix -------------------------------------------------------
# Distro packaging:   qmake6 PREFIX=/usr   &&  make install INSTALL_ROOT=$pkgdir
# Personal (no sudo): qmake6 PREFIX=$HOME/.local  &&  make install
isEmpty(PREFIX): PREFIX = /usr/local

target.path       = $$PREFIX/bin

desktopfile.path  = $$PREFIX/share/applications
desktopfile.files = qfontviewer.desktop

icon.path         = $$PREFIX/share/icons/hicolor/scalable/apps
icon.files        = qfontviewer.svg

INSTALLS += target desktopfile icon
