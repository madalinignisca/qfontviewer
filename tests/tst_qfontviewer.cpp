// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#include <QtTest>
#include <QStringListModel>
#include <QFontDatabase>
#include <QFont>

#include "fontfilterproxy.h"
#include "glyphgrid.h"
#include "fontmetrics.h"

class TestQFontViewer : public QObject {
    Q_OBJECT

    static bool hasDejaVuMono()
    {
        return QFontDatabase::families().contains(QStringLiteral("DejaVu Sans Mono"));
    }

private slots:
    // Name filtering works on arbitrary strings (no font lookup involved).
    void filterByName()
    {
        QStringListModel model(QStringList { "Alpha Mono", "Beta Sans", "Gamma Mono", "Delta" });
        FontFilterProxy proxy;
        proxy.setSourceModel(&model);
        QCOMPARE(proxy.rowCount(), 4);

        proxy.setNameFilter("Mono");
        QCOMPARE(proxy.rowCount(), 2);

        proxy.setNameFilter("delta"); // case-insensitive
        QCOMPARE(proxy.rowCount(), 1);

        proxy.setNameFilter(QString());
        QCOMPARE(proxy.rowCount(), 4);
    }

    // Monospace-only must yield a fixed-pitch subset of the real font list.
    void filterMonospace()
    {
        if (!hasDejaVuMono())
            QSKIP("DejaVu Sans Mono not installed");

        QStringListModel model(QFontDatabase::families());
        FontFilterProxy proxy;
        proxy.setSourceModel(&model);
        const int all = proxy.rowCount();

        proxy.setMonospaceOnly(true);
        const int mono = proxy.rowCount();
        QVERIFY(mono > 0);
        QVERIFY(mono <= all);
        for (int i = 0; i < mono; ++i)
            QVERIFY(QFontDatabase::isFixedPitch(proxy.index(i, 0).data().toString()));
    }

    void glyphModelCoverage()
    {
        if (!hasDejaVuMono())
            QSKIP("DejaVu Sans Mono not installed");

        GlyphModel m;
        m.setFamily(QStringLiteral("DejaVu Sans Mono"));
        QVERIFY(m.rowCount() > 0);

        const int row = m.rowOfCodepoint(0x5F); // underscore
        QVERIFY(row >= 0);
        QCOMPARE(m.codepointAt(row), uint(0x5F));

        QCOMPARE(m.rowOfCodepoint(0x10FFFF), -1); // not a real glyph
    }

    void analyzeClipBasics()
    {
        if (!hasDejaVuMono())
            QSKIP("DejaVu Sans Mono not installed");

        QFont f(QStringLiteral("DejaVu Sans Mono"));
        f.setPixelSize(16);
        const qfv::ClipInfo ci = qfv::analyzeClip(f, QStringLiteral("_"));

        QVERIFY(ci.ascent > 0.0);
        QVERIFY(ci.descent >= 0.0);
        QVERIFY(ci.height >= ci.ascent + ci.descent - 1.0);
        QVERIFY(ci.glyphBottom > 0.0); // underscore sits below the baseline
        QCOMPARE(ci.cellBottom, ci.descent);
    }
};

QTEST_MAIN(TestQFontViewer)
#include "tst_qfontviewer.moc"
