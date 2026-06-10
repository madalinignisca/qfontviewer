// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#include "previewwidget.h"
#include "fontmetrics.h"

#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QPaintEvent>

// Build the font exactly as the controls dictate. NoFontMerging is ALWAYS set
// so missing glyphs are never substituted from a fallback font -- the preview
// (and the glyph coverage) must reflect the selected family only.
static QFont buildFont(const QString &family, int px, bool aa, bool hint)
{
    QFont f(family);
    f.setPixelSize(px);

    QFont::StyleStrategy ss = aa ? QFont::PreferAntialias : QFont::NoAntialias;
    ss = QFont::StyleStrategy(ss | QFont::NoFontMerging);
    f.setStyleStrategy(ss);

    // HintingPreference is a plain enum, not flags.
    f.setHintingPreference(hint ? QFont::PreferFullHinting : QFont::PreferNoHinting);
    return f;
}

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
    recompute();
}

void PreviewWidget::setFamily(const QString &family)
{
    m_family = family;
    recompute();
    update();
}
void PreviewWidget::setText(const QString &text)
{
    m_text = text;
    recompute();
    update();
}
void PreviewWidget::setPixelSize(int px)
{
    m_pixelSize = px;
    recompute();
    update();
}
void PreviewWidget::setZoom(int z)
{
    m_zoom = qMax(1, z);
    recompute();
    update();
}
void PreviewWidget::setAntialias(bool on)
{
    m_antialias = on;
    recompute();
    update();
}
void PreviewWidget::setHinting(bool on)
{
    m_hinting = on;
    recompute();
    update();
}
void PreviewWidget::setFocusChar(const QString &ch)
{
    m_focus = ch;
    recompute();
    update();
}

void PreviewWidget::recompute()
{
    if (m_family.isEmpty()) {
        setMinimumSize(0, 0);
        emit metricsUpdated(QString());
        return;
    }

    const QFont f = buildFont(m_family, m_pixelSize, m_antialias, m_hinting);

    // Objective clipping verdict for the focus character (see qfv::analyzeClip).
    const qfv::ClipInfo ci = qfv::analyzeClip(f, m_focus);

    const QString report = QStringLiteral(
        "px %1   ascent %2  descent %3  height %4    "
        "'%5': glyph-bottom %6 vs cell-bottom %7   →  touches/exceeds cell bottom: %8")
                               .arg(m_pixelSize)
                               .arg(ci.ascent, 0, 'f', 1)
                               .arg(ci.descent, 0, 'f', 1)
                               .arg(ci.height, 0, 'f', 1)
                               .arg(m_focus)
                               .arg(ci.glyphBottom, 0, 'f', 1)
                               .arg(ci.cellBottom, 0, 'f', 1)
                               .arg(ci.clips ? QStringLiteral("YES") : QStringLiteral("no"));
    emit metricsUpdated(report);

    if (!m_antialias) {
        const QFontMetrics fmi(f);
        QString line = m_text;
        line.replace(QLatin1Char('\n'), QLatin1Char(' '));
        const int w1 = fmi.horizontalAdvance(line) + 2;
        const int h1 = fmi.height() + 2;
        setMinimumSize(w1 * m_zoom + 16, h1 * m_zoom + 16);
    } else {
        setMinimumSize(0, 0);
    }
    updateGeometry();
}

void PreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::white);

    if (m_family.isEmpty()) {
        p.setPen(Qt::gray);
        p.drawText(rect(), Qt::AlignCenter, tr("Select a font"));
        return;
    }

    const QFont f = buildFont(m_family, m_pixelSize, m_antialias, m_hinting);

    if (m_antialias) {
        // Normal mode: faithful antialiased rendering, hints follow the toggle.
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::TextAntialiasing, true);
        p.setFont(f);
        p.setPen(Qt::black);
        p.drawText(rect().adjusted(8, 8, -8, -8), Qt::TextWordWrap | Qt::AlignTop, m_text);
        return;
    }

    // Console mode: render one line at 1x with all smoothing off.
    const QFontMetrics fm(f);
    QString line = m_text;
    line.replace(QLatin1Char('\n'), QLatin1Char(' '));
    const int w1 = qMax(1, fm.horizontalAdvance(line) + 2);
    const int h1 = qMax(1, fm.height() + 2);

    QImage img(w1, h1, QImage::Format_ARGB32);
    img.fill(Qt::white);
    {
        QPainter ip(&img);
        ip.setRenderHint(QPainter::Antialiasing, false);
        ip.setRenderHint(QPainter::TextAntialiasing, false);
        ip.setFont(f);
        ip.setPen(Qt::black);
        ip.drawText(1, fm.ascent(), line); // integer baseline, no subpixel
    }

    // Magnify nearest-neighbor (FastTransformation is a QImage::scaled() arg).
    const QImage big
        = img.scaled(w1 * m_zoom, h1 * m_zoom, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    const int ox = 8, oy = 8;
    p.drawImage(ox, oy, big);

    // Overlay: cell top (green), baseline (blue), cell bottom (red).
    const int z = m_zoom;
    const int topY = oy + 1 * z; // baseline - ascent
    const int baseY = oy + (1 + fm.ascent()) * z;
    const int botY = oy + (1 + fm.ascent() + fm.descent()) * z;
    const int right = ox + big.width();

    p.setPen(QPen(QColor(0, 170, 0), 1));
    p.drawLine(ox, topY, right, topY);
    p.setPen(QPen(QColor(0, 128, 255), 1));
    p.drawLine(ox, baseY, right, baseY);
    p.setPen(QPen(QColor(220, 0, 0), 1));
    p.drawLine(ox, botY, right, botY);
}
