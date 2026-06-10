# Changelog

All notable changes to this project are documented here. The format is based on
[Keep a Changelog](https://keepachangelog.com/), and the project adheres to
[Semantic Versioning](https://semver.org/).

## [1.2.0] - 2026-06-10

### Added
- AppStream metainfo (`io.github.madalinignisca.qfontviewer.metainfo.xml`).
- Internationalization: Qt Linguist infrastructure with an embedded Romanian
  starter translation; the UI loads the system-locale translation at startup.
- `--version` and `--help` command-line options.

### Changed
- Adopted a **reverse-DNS application ID** `io.github.madalinignisca.qfontviewer`
  (the `.desktop`, icon, and Wayland app_id are renamed accordingly). If you target
  the app in a tiling WM, update the app_id, e.g. Sway
  `for_window [app_id="io.github.madalinignisca.qfontviewer"]`.

## [1.1.0] - 2026-06-10

### Changed
- **Build system migrated from qmake6 to CMake** (≥ 3.21). The `.pro` is removed;
  packaging (AUR/Debian/Fedora) updated accordingly. No application behavior change.

### Added
- QtTest unit tests (`tests/`) covering font filtering, glyph coverage, and the
  clipping analysis; runnable via `ctest`.
- `.clang-format` and `.clang-tidy` configurations.
- GitHub Actions CI: builds on Ubuntu + Arch, runs the tests and a clang-format
  check (blocking), plus clang-tidy and clazy (advisory).
- Extracted the clipping computation into a pure, testable `qfv::analyzeClip`.

## [1.0.0] - 2026-06-10

### Added
- Initial release.
- Font list with substring filter and monospace-only toggle.
- Preview with editable sample text, pixel size, and zoom.
- Console-approximation mode: antialiasing/hinting off, nearest-neighbor
  magnification, cell-top/baseline/cell-bottom overlay, and an objective
  "touches/exceeds cell bottom" clipping readout for the focus character.
- Glyph grid showing the characters a family contains (incl. box-drawing and
  block elements), with a "go to codepoint" jump and an enlarged detail view.
- Side-by-side comparison of two font families.
- qmake6 build with `PREFIX`/`INSTALL_ROOT` support; `.desktop` entry and icon.
- Packaging for Arch (AUR), Debian, and Fedora.

[1.2.0]: https://github.com/madalinignisca/qfontviewer/releases/tag/v1.2.0
[1.1.0]: https://github.com/madalinignisca/qfontviewer/releases/tag/v1.1.0
[1.0.0]: https://github.com/madalinignisca/qfontviewer/releases/tag/v1.0.0
