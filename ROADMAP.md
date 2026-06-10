# Roadmap

Direction for `qfontviewer` after the 1.0.0 release. Items are grouped by the
value they add; order within a tier is rough priority.

## Tier 1 — code quality & "modern Qt6/C++" foundations ✅ (v1.1.0)

- [x] **CMake as the build** — `CMakeLists.txt` replaced qmake6 entirely.
- [x] **clang-format** — committed `.clang-format`; CI format check (blocking).
- [x] **clang-tidy** — `.clang-tidy` (bugprone / performance / modernize / core guidelines).
- [x] **clazy** — Qt-specific static analysis in CI (advisory).
- [x] **QtTest unit tests** (`tests/`) — `FontFilterProxy` filtering, `GlyphModel`
      coverage, and the clipping verdict (`qfv::analyzeClip`); wired into `ctest`.
- [x] **GitHub Actions CI** — builds on Ubuntu + Arch, runs format check + tests
      (blocking) and clang-tidy + clazy (advisory); status badge in the README.

## Tier 2 — CI depth & repository hygiene ✅

- [x] `.editorconfig`.
- [x] Community files: `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md` (Contributor Covenant
      2.1), `SECURITY.md`, issue forms + PR template under `.github/`.
- [x] `cppcheck` (advisory CI). IWYU intentionally skipped in favor of clang-tidy's
      `misc-include-cleaner`.
- [x] `pre-commit` hooks (clang-format 22.1.5 + hygiene); CI runs `pre-commit run --all-files`.
- [x] REUSE compliance (`LICENSES/` + `REUSE.toml` + `reuse lint` CI job + badge);
      SPDX headers canonicalized to `SPDX-FileCopyrightText`.

## Tier 3 — Linux-desktop distribution polish

- [x] AppStream **metainfo** (`data/io.github.madalinignisca.qfontviewer.metainfo.xml`),
      validated in CI. (Screenshots still to add once an image is hosted.)
- [x] Reverse-DNS app ID `io.github.madalinignisca.qfontviewer` (desktop/icon/app_id).
- [x] Internationalization: `lupdate`/`lrelease` via CMake, `.qm` embedded under
      `:/i18n/`, loaded at startup; Romanian starter translation.
- [x] `--version` / `--help` CLI.
- [ ] Refined / multi-size icon (current SVG is scalable; low priority).
- [ ] **Flatpak / Flathub** — *needs discussion before any submission.* Flathub
      maintainers have pushed back on low-effort app submissions; evaluate whether
      Flathub is the right channel and how to present/maintain the app properly.
      (The reverse-DNS ID prerequisite is now done.)

---

Tiers 2 and 3 are committed direction, not afterthoughts — they ship after Tier 1
lands and CI is green.
