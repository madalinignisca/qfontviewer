// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

#include "fontfilterproxy.h"
#include <QFontDatabase>

FontFilterProxy::FontFilterProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

void FontFilterProxy::setMonospaceOnly(bool on)
{
    if (m_monoOnly == on)
        return;
    m_monoOnly = on;
    invalidate();
}

void FontFilterProxy::setNameFilter(const QString &text)
{
    if (m_name == text)
        return;
    m_name = text;
    invalidate();
}

bool FontFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
    const QString family = idx.data(Qt::DisplayRole).toString();

    if (!m_name.isEmpty() && !family.contains(m_name, Qt::CaseInsensitive))
        return false;

    // QFontDatabase is fully static in Qt6.
    if (m_monoOnly && !QFontDatabase::isFixedPitch(family))
        return false;

    return true;
}
