// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#pragma once
#include <QString>

class QFont;

namespace qfv {

// Result of measuring one character against its font cell. All vertical
// measures are in pixels; glyphBottom/cellBottom are distances below the
// baseline (positive = downward).
struct ClipInfo {
    double ascent = 0.0;
    double descent = 0.0;
    double height = 0.0;
    double glyphBottom = 0.0; // tight glyph bbox bottom, below baseline
    double cellBottom = 0.0; // the cell's bottom edge, below baseline
    bool clips = false; // glyph reaches/exceeds the cell bottom
};

// Measure how a focus character (e.g. "_") sits in its cell for the given font.
// Pure and side-effect free, so it is directly unit-testable.
ClipInfo analyzeClip(const QFont &font, const QString &focusChar);

} // namespace qfv
