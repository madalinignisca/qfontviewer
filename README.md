# qfontviewer

**A font viewer that uses only minimal Qt dependencies.**

`qfontviewer` is a small, native desktop app for browsing the fonts installed on
your system and seeing exactly how they render — including a **console / terminal
rendering preview** that predicts whether glyphs (like the underscore `_`) get
clipped at small pixel sizes, *before* you commit a font to a real console.

It was written specifically to avoid pulling in a full desktop environment just to
look at fonts: it links **only `qt6-base`** (Qt 6 Widgets). No GNOME or KDE
libraries, no QML runtime, no Python — just one Qt library and a ~150 KB binary.

[![CI](https://github.com/madalinignisca/qfontviewer/actions/workflows/ci.yml/badge.svg)](https://github.com/madalinignisca/qfontviewer/actions/workflows/ci.yml)
[![REUSE status](https://api.reuse.software/badge/github.com/madalinignisca/qfontviewer)](https://api.reuse.software/info/github.com/madalinignisca/qfontviewer)
![license](https://img.shields.io/badge/license-GPL--3.0--or--later-blue)
![Qt](https://img.shields.io/badge/Qt-6%20Widgets-41cd52)
![deps](https://img.shields.io/badge/runtime%20deps-qt6--base%20only-brightgreen)

## Why

Most font viewers either belong to a desktop environment (and drag in dozens of
its libraries) or render fonts only the way *that toolkit* draws them. If you live
in a tiling-WM / console-centric setup and just want to **pick a clean monospace
font for your terminal or VT**, that's a lot of baggage for a simple question.

`qfontviewer` answers that question directly, with the smallest possible dependency
footprint.

## Features

- **Font list** with live substring filter and a **Monospace-only** toggle.
- **Preview** with editable sample text, pixel size, and live size control.
- **Console-approximation mode** *(default)* — antialiasing and hinting off,
  rendered at a chosen *pixel* size and magnified nearest-neighbor so individual
  lit pixels are visible, with **cell-top / baseline / cell-bottom** overlay lines
  and an objective **"_ touches/exceeds cell bottom: yes/no"** readout. Flip
  *Antialias* on for a normal, faithful desktop preview.
- **Glyph grid** of the characters a family actually contains (ASCII, Latin,
  punctuation, arrows, math, **box-drawing & block elements**), a "go to codepoint"
  jump (`5F`, `U+2500`), and an enlarged detail view.
- **Side-by-side compare** of two families sharing the same text and size.

> The console mode *approximates* a monochrome single-face console renderer (e.g.
> kmscon/FreeType): it disables antialiasing, font fallback (`NoFontMerging`), and
> subpixel positioning. It predicts clipping reliably but is not byte-identical to
> any specific console's engine.

## Requirements

| | |
|---|---|
| **Runtime** | `qt6-base` (Qt 6 Widgets) — and `qt6-wayland` if you run Wayland |
| **Build**   | a C++17 compiler, **CMake ≥ 3.21**, Ninja or Make |

## Build & install

### From source (CMake)

```sh
git clone https://github.com/madalinignisca/qfontviewer.git
cd qfontviewer
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure   # optional: run the unit tests
sudo cmake --install build                    # default prefix /usr/local
```

**Personal install, no sudo** — install under your home directory:

```sh
cmake -B build -G Ninja -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build
cmake --install build                         # → ~/.local/bin, ~/.local/share/...
```

See [`BUILDING.md`](BUILDING.md) for options (`BUILD_TESTING`, prefixes) and the
linter/CI commands.

### Packages

Ready-made packaging lives under [`packaging/`](packaging/):

- **Arch Linux (AUR)** — `packaging/aur/` (`qfontviewer` and `qfontviewer-git`)
- **Debian/Ubuntu** — `packaging/debian/`
- **Fedora** — `packaging/fedora/qfontviewer.spec`

See [`packaging/README.md`](packaging/README.md) for per-distro build instructions.

## Usage

Launch `qfontviewer` (it installs a `.desktop` entry, so it also shows up in app
launchers / `wmenu`). Pick a font on the left; the **Preview** tab shows it, the
**Glyphs** tab lists its glyphs. To check a console font: select it, set a small
pixel size with *Antialias* off, and watch whether the `_` crosses the red
cell-bottom line.

The Wayland `app_id` is `qfontviewer`, so tiling-WM users can target it directly
(e.g. in Sway: `for_window [app_id="qfontviewer"] floating enable`).

## Contributing

Issues and pull requests are welcome. The codebase is deliberately small and plain
Qt6 Widgets C++17 — no extra dependencies, please, to keep the "minimal" promise.
See [`CONTRIBUTING.md`](CONTRIBUTING.md) for the workflow (build/test/`pre-commit`),
the [Code of Conduct](CODE_OF_CONDUCT.md), and [`SECURITY.md`](SECURITY.md) for
reporting vulnerabilities. Build/linting details are in [`BUILDING.md`](BUILDING.md);
CI runs tests, `pre-commit` (clang-format), `reuse lint`, and clang-tidy/clazy/cppcheck.
The project is [REUSE](https://reuse.software)-compliant.

## License

Licensed under the **GNU General Public License v3.0 or later**
(`GPL-3.0-or-later`) — the license Qt applications such as Qt Creator use. (The Qt
*libraries* are offered under the LGPL, whose extra permissions exist so software
can *link* them; this is an application, so it carries the full GPL.) The complete
text is in [`COPYING`](COPYING).

Copyright © 2026 Madalin Ignisca &lt;git@madalin.me&gt;
