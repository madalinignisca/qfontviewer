# Changelog

All notable changes to this project are documented here. The format is based on
[Keep a Changelog](https://keepachangelog.com/), and the project adheres to
[Semantic Versioning](https://semver.org/).

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

[1.0.0]: https://github.com/madalinignisca/qfontviewer/releases/tag/v1.0.0
