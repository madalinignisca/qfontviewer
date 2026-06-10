# Roadmap

Direction for `qfontviewer` after the 1.0.0 release. Items are grouped by the
value they add; order within a tier is rough priority.

## Tier 1 — code quality & "modern Qt6/C++" foundations

- [ ] **CMake as a first-class build** — promote the `BUILDING.md` recipe to a real
      `CMakeLists.txt`; keep qmake6 working in parallel during transition.
- [ ] **clang-format** — committed `.clang-format`; CI format check.
- [ ] **clang-tidy** — `.clang-tidy` (bugprone / performance / modernize / core guidelines).
- [ ] **clazy** — Qt-specific static analysis in CI.
- [ ] **QtTest unit tests** (`tests/`) — cover the non-GUI logic: `FontFilterProxy`
      filtering, `GlyphModel` coverage, the clipping verdict. Wire into `ctest`.
- [ ] **GitHub Actions CI** — build qmake6 + cmake, run format check, clang-tidy,
      clazy, and tests; status badge in the README.

## Tier 2 — CI depth & repository hygiene

- [ ] `.editorconfig`.
- [ ] Community files: `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`,
      issue/PR templates under `.github/`.
- [ ] `cppcheck` and include-what-you-use passes (alongside clang-tidy).
- [ ] `pre-commit` hooks running clang-format.
- [ ] REUSE compliance (`LICENSES/` dir + `reuse lint`); SPDX headers already present.

## Tier 3 — Linux-desktop distribution polish

- [ ] AppStream **metainfo** (`*.metainfo.xml`) with screenshots & description.
- [ ] Internationalization: `lupdate`/`lrelease` workflow + initial `.ts` files
      (the UI already uses `tr()` throughout).
- [ ] `--version` CLI flag; refined / multi-size icon.
- [ ] **Flatpak / Flathub** — *needs discussion before any submission.* Flathub
      maintainers have pushed back on low-effort app submissions; evaluate whether
      Flathub is the right channel and how to present/maintain the app properly.

---

Tiers 2 and 3 are committed direction, not afterthoughts — they ship after Tier 1
lands and CI is green.
