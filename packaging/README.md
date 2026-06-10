# Packaging

Distro packaging for `qfontviewer`. All builds use the upstream qmake6 build with
`PREFIX=/usr` and stage into the packaging tool's buildroot.

## Arch Linux (AUR) — `aur/`

Two PKGBUILDs:

- **`aur/PKGBUILD`** — `qfontviewer`, builds a tagged release tarball from GitHub.
- **`aur/PKGBUILD-git`** — `qfontviewer-git`, builds the latest `main`.

Local build / install:

```sh
cd packaging/aur
makepkg -si                         # release
# or, for the VCS package:
cp PKGBUILD-git /tmp/qfontviewer-git/PKGBUILD && cd /tmp/qfontviewer-git && makepkg -si
```

Submitting to the AUR (release package):

```sh
# after pushing a v1.0.0 tag to GitHub so the tarball exists:
cd packaging/aur
updpkgsums                          # fill in the real sha256sum
makepkg --printsrcinfo > .SRCINFO
# then push PKGBUILD + .SRCINFO to ssh://aur@aur.archlinux.org/qfontviewer.git
```

The VCS package may use `sha256sums=('SKIP')` and is submittable immediately
(no release needed).

## Debian / Ubuntu — `debian/`

```sh
sudo apt install build-essential debhelper qt6-base-dev
# from the repo root, with the debian/ dir in place:
cp -r packaging/debian debian
dpkg-buildpackage -us -uc -b
```

Produces `../qfontviewer_1.0.0-1_<arch>.deb`. Uses the debhelper `qmake6`
buildsystem (debhelper >= 13). `debian/source/format` is `3.0 (quilt)`, so a
matching `../qfontviewer_1.0.0.orig.tar.gz` is needed for a full source build
(`git archive` or `gbp`); for a quick binary `.deb`, `-b` as above is enough.

## Fedora — `fedora/qfontviewer.spec`

```sh
sudo dnf install rpm-build qt6-qtbase-devel gcc-c++ desktop-file-utils
# place the release tarball in ~/rpmbuild/SOURCES/ (e.g. from `git archive`):
rpmbuild -ba packaging/fedora/qfontviewer.spec
```

Produces an RPM under `~/rpmbuild/RPMS/`.
