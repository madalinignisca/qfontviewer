Name:           qfontviewer
Version:        1.0.0
Release:        1%{?dist}
Summary:        Minimal Qt6 font viewer with a console-rendering preview

License:        GPL-3.0-or-later
URL:            https://github.com/madalinignisca/qfontviewer
Source0:        %{url}/archive/refs/tags/v%{version}.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  qt6-qtbase-devel
BuildRequires:  desktop-file-utils

Requires:       qt6-qtbase

%description
qfontviewer is a small Qt6 Widgets application for browsing installed fonts. It
links only qt6-base (no GNOME/KDE libraries, no QML, no Python).

Its console-approximation mode renders a font at a chosen pixel size with
antialiasing and hinting disabled, magnified nearest-neighbor, with overlay lines
and an objective clipping readout - so you can tell whether glyphs such as the
underscore get clipped at small sizes before committing a font to a terminal or
virtual console. It also offers a glyph grid and side-by-side comparison of two
families.

%prep
%autosetup

%build
qmake6 PREFIX=%{_prefix}
%make_build

%install
# qmake's generated Makefile uses INSTALL_ROOT (not DESTDIR) for staging.
make install INSTALL_ROOT=%{buildroot}

%check
desktop-file-validate %{buildroot}%{_datadir}/applications/%{name}.desktop

%files
%license COPYING
%doc README.md CHANGELOG.md
%{_bindir}/qfontviewer
%{_datadir}/applications/qfontviewer.desktop
%{_datadir}/icons/hicolor/scalable/apps/qfontviewer.svg

%changelog
* Wed Jun 10 2026 Madalin Ignisca <git@madalin.me> - 1.0.0-1
- Initial package
