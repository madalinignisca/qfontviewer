// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

#pragma once
#include <QSortFilterProxyModel>
#include <QString>

// Filters the flat list of font families by (a) a case-insensitive substring
// of the family name and (b) optionally only fixed-pitch (monospace) families.
class FontFilterProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit FontFilterProxy(QObject *parent = nullptr);

    void setMonospaceOnly(bool on);
    void setNameFilter(const QString &text);

protected:
    [[nodiscard]] bool filterAcceptsRow(
        int sourceRow, const QModelIndex &sourceParent) const override;

private:
    bool m_monoOnly = false;
    QString m_name;
};
