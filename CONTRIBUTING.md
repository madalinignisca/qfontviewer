# Contributing to qfontviewer

Thanks for your interest! qfontviewer is intentionally small and focused, and the
guiding constraint is the project's reason to exist:

> **Minimal dependencies — the app links only `qt6-base` (Qt 6 Widgets).**
> Please don't introduce new runtime dependencies (no extra Qt modules, no
> third-party libraries). Header-only test helpers are fine; runtime deps are not.

## Getting started

```sh
git clone https://github.com/madalinignisca/qfontviewer.git
cd qfontviewer
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
```

See [`BUILDING.md`](BUILDING.md) for options, prefixes, and the linter commands.

## Before opening a pull request

1. **Format & hooks.** Install the hooks once and let them run:
   ```sh
   pipx install pre-commit   # or: pip install --user pre-commit
   pre-commit install
   pre-commit run --all-files
   ```
   Formatting uses **clang-format 22.1.5** (pinned); CI enforces it.
2. **Tests pass.** `ctest --test-dir build --output-on-failure`.
3. **Keep it warning-clean.** The build uses `-Wall -Wextra`. clang-tidy, clazy, and
   cppcheck run in CI (advisory) — please glance at new findings on your code.
4. **One focused change per PR**, with a clear description. Update `CHANGELOG.md`
   under an "Unreleased" heading if your change is user-visible.

## Coding style

- C++17, Qt 6 Widgets, 4-space indent (see `.clang-format` / `.editorconfig`).
- Every source file starts with the SPDX header:
  ```cpp
  // SPDX-License-Identifier: GPL-3.0-or-later
  // SPDX-FileCopyrightText: <year> <your name> <your email>
  ```
  The project is **REUSE-compliant** (`reuse lint` runs in CI); new files must carry
  an SPDX header or be covered by `REUSE.toml`.

## Translations

The UI uses Qt Linguist. To add a language, create a `.ts` for it and wire it into
the build:

```sh
/usr/lib/qt6/bin/lupdate src/ -ts translations/qfontviewer_<lang>.ts   # e.g. _de, _fr
# edit the .ts in Qt Linguist (or by hand), then add it to the
# qt_add_lrelease(TS_FILES ...) list in CMakeLists.txt
```

`.qm` files are compiled and embedded under `:/i18n/`; the app loads the system
locale's translation at startup. A Romanian (`ro`) starter translation is included.

## Reporting bugs / requesting features

Use the GitHub issue templates. For security issues, see [`SECURITY.md`](SECURITY.md)
— please do **not** open a public issue for vulnerabilities.

By contributing, you agree your contributions are licensed under the project's
**GPL-3.0-or-later** license, and you abide by the
[Code of Conduct](CODE_OF_CONDUCT.md).
