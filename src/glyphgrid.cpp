// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#include "glyphgrid.h"

#include <QListView>
#include <QLabel>
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QFontMetricsF>

namespace {

struct Range {
    uint lo;
    uint hi;
};

// Useful for evaluating console/monospace fonts: ASCII, Latin, punctuation,
// arrows, math, and (importantly) box-drawing + block elements.
const Range kRanges[] = {
    { 0x0020, 0x007E },
    { 0x00A0, 0x00FF },
    { 0x0100, 0x024F },
    { 0x2000, 0x206F },
    { 0x2190, 0x21FF },
    { 0x2200, 0x22FF },
    { 0x2500, 0x257F },
    { 0x2580, 0x259F },
};

// Paints each cell: the glyph (large) over its U+XXXX label (small).
class GlyphDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QString family;
    int glyphPx = 30;

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        return QSize(60, 70);
    }

    void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &idx) const override
    {
        const bool sel = opt.state & QStyle::State_Selected;
        p->fillRect(opt.rect, sel ? opt.palette.highlight() : QBrush(Qt::white));

        const QColor fg = sel ? opt.palette.highlightedText().color() : QColor(Qt::black);
        const uint cp = idx.data(GlyphModel::CodepointRole).toUInt();
        const QString ch = idx.data(Qt::DisplayRole).toString();

        // Glyph -- drawn only from the selected family (NoFontMerging) so a
        // missing glyph shows blank rather than a fallback substitute.
        QFont gf(family);
        gf.setPixelSize(glyphPx);
        gf.setStyleStrategy(QFont::NoFontMerging);
        p->setFont(gf);
        p->setPen(fg);
        QRect glyphRect = opt.rect.adjusted(0, 2, 0, -16);
        p->drawText(glyphRect, Qt::AlignCenter, ch);

        // Codepoint label.
        QFont lf = opt.font;
        lf.setPixelSize(10);
        p->setFont(lf);
        p->setPen(sel ? fg : QColor(120, 120, 120));
        QRect labelRect(opt.rect.left(), opt.rect.bottom() - 15, opt.rect.width(), 14);
        p->drawText(labelRect, Qt::AlignCenter,
            QStringLiteral("U+%1").arg(cp, 4, 16, QLatin1Char('0')).toUpper());
    }
};

} // namespace

// ----------------------------------------------------------------------------
// GlyphModel
// ----------------------------------------------------------------------------
GlyphModel::GlyphModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void GlyphModel::setFamily(const QString &family)
{
    beginResetModel();
    m_family = family;
    m_cps.clear();

    if (!family.isEmpty()) {
        QFont f(family);
        f.setStyleStrategy(QFont::NoFontMerging); // coverage of THIS family only
        const QFontMetrics fm(f);
        for (const Range &r : kRanges)
            for (uint cp = r.lo; cp <= r.hi; ++cp)
                if (fm.inFontUcs4(cp))
                    m_cps.append(cp);
    }
    endResetModel();
}

int GlyphModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_cps.size();
}

QVariant GlyphModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_cps.size())
        return { };
    const uint cp = m_cps.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return QString::fromUcs4(reinterpret_cast<const char32_t *>(&cp), 1);
    case CodepointRole:
        return cp;
    default:
        return { };
    }
}

int GlyphModel::rowOfCodepoint(uint cp) const
{
    return m_cps.indexOf(cp);
}
uint GlyphModel::codepointAt(int row) const
{
    return (row >= 0 && row < m_cps.size()) ? m_cps.at(row) : 0;
}

// ----------------------------------------------------------------------------
// GlyphGrid
// ----------------------------------------------------------------------------
GlyphGrid::GlyphGrid(QWidget *parent)
    : QWidget(parent)
{
    m_model = new GlyphModel(this);

    m_view = new QListView(this);
    m_view->setModel(m_model);
    auto *delegate = new GlyphDelegate(m_view);
    m_view->setItemDelegate(delegate);
    m_delegate = delegate;
    m_view->setViewMode(QListView::IconMode);
    m_view->setResizeMode(QListView::Adjust);
    m_view->setMovement(QListView::Static);
    m_view->setUniformItemSizes(true);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);

    m_detail = new QLabel(this);
    m_detail->setAlignment(Qt::AlignCenter);
    m_detail->setMinimumHeight(64);
    m_detail->setFrameShape(QFrame::StyledPanel);

    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(m_view, 1);
    lay->addWidget(m_detail, 0);

    connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged, this,
        &GlyphGrid::onCurrentChanged);
}

void GlyphGrid::setFamily(const QString &family)
{
    m_family = family;
    static_cast<GlyphDelegate *>(m_delegate)->family = family; // we created it
    m_model->setFamily(family);
    m_detail->clear();
}

void GlyphGrid::gotoCodepoint(uint cp)
{
    const int row = m_model->rowOfCodepoint(cp);
    if (row < 0) {
        m_detail->setText(tr("U+%1 not in this font").arg(cp, 4, 16, QLatin1Char('0')).toUpper());
        return;
    }
    const QModelIndex idx = m_model->index(row, 0);
    m_view->setCurrentIndex(idx);
    m_view->scrollTo(idx, QAbstractItemView::PositionAtCenter);
}

void GlyphGrid::onCurrentChanged(const QModelIndex &cur)
{
    if (!cur.isValid())
        return;
    const uint cp = cur.data(GlyphModel::CodepointRole).toUInt();
    const QString ch = cur.data(Qt::DisplayRole).toString();

    QFont bigf(m_family);
    bigf.setPixelSize(40);
    bigf.setStyleStrategy(QFont::NoFontMerging);
    m_detail->setFont(bigf);
    m_detail->setText(
        QStringLiteral("%1    U+%2").arg(ch).arg(cp, 4, 16, QLatin1Char('0')).toUpper());
}
