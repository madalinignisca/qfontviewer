# Security Policy

## Supported versions

qfontviewer is a small desktop utility; security fixes are made against the latest
release and `main`.

| Version | Supported |
|---------|-----------|
| latest release / `main` | ✅ |
| older releases | ❌ |

## Reporting a vulnerability

Please report security issues **privately**, not via public GitHub issues.

- Preferred: GitHub's **private vulnerability reporting** (the *Report a vulnerability*
  button under the repository's **Security** tab).
- Or email **git@madalin.me** with details and, if possible, steps to reproduce.

You'll get an acknowledgement as soon as practical. Once a fix is available, it will
be released and the report credited (unless you prefer to remain anonymous).

Given the app's nature (it reads font metadata via Qt and renders glyphs locally, with
no network access), the most relevant concerns are crashes or memory-safety issues
triggered by malformed fonts — those are in scope and welcome.
