// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#include "fontmetrics.h"

#include <QFont>
#include <QFontMetricsF>
#include <QRectF>

namespace qfv {

ClipInfo analyzeClip(const QFont &font, const QString &focusChar)
{
    const QFontMetricsF fm(font);

    ClipInfo ci;
    ci.ascent = fm.ascent();
    ci.descent = fm.descent();
    ci.height = fm.height();
    // tightBoundingRect is baseline-relative; bottom() > 0 means the glyph
    // extends below the baseline (true for "_").
    ci.glyphBottom = fm.tightBoundingRect(focusChar).bottom();
    ci.cellBottom = ci.descent;
    ci.clips = ci.glyphBottom >= ci.cellBottom - 0.01;
    return ci;
}

} // namespace qfv
