## Summary

<!-- What does this change, and why? -->

## Checklist

- [ ] Builds clean (`cmake --build build`, no new `-Wall -Wextra` warnings)
- [ ] Tests pass (`ctest --test-dir build --output-on-failure`)
- [ ] `pre-commit run --all-files` is clean (clang-format 22.1.5)
- [ ] No new runtime dependencies (the app links only `qt6-base`)
- [ ] SPDX header on any new files (the project is REUSE-compliant)
- [ ] `CHANGELOG.md` updated if the change is user-visible
