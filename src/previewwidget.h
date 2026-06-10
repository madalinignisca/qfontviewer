// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

#pragma once
#include <QWidget>
#include <QString>

// Renders a sample string in a chosen font. Two modes, driven by the AA toggle:
//   * Console mode (AA off): renders into a 1x QImage with all smoothing off,
//     then magnifies nearest-neighbor so individual lit pixels are visible,
//     with baseline / cell-bottom overlay lines. Approximates kmscon's
//     monochrome engine to predict glyph clipping (e.g. underscore).
//   * Normal mode (AA on): faithful antialiased desktop preview, word-wrapped.
// Emits metricsUpdated() with an objective clipping report for the focus char.
class PreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

    QString family() const { return m_family; }

public slots:
    void setFamily(const QString &family);
    void setText(const QString &text);
    void setPixelSize(int px);
    void setZoom(int z);
    void setAntialias(bool on);
    void setHinting(bool on);
    void setFocusChar(const QString &ch);

signals:
    void metricsUpdated(const QString &report);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    void recompute(); // recompute min size + emit the metrics report

    QString m_family;
    QString m_text = QStringLiteral("The quick brown fox 0123 _____ |il1 oO0 g_y");
    int m_pixelSize = 16;
    int m_zoom = 6;
    bool m_antialias = false; // start in console mode (the point of the app)
    bool m_hinting = true; // kmscon uses full hinting
    QString m_focus = QStringLiteral("_");
};
