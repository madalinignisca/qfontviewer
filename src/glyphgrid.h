// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Madalin Ignisca <git@madalin.me>

#pragma once
#include <QWidget>
#include <QAbstractListModel>
#include <QList>
#include <QString>

class QListView;
class QLabel;
class QStyledItemDelegate;

// Model of the code points the selected family actually contains (tested with
// QFontMetrics::inFontUcs4 over a handful of useful Unicode ranges).
class GlyphModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { CodepointRole = Qt::UserRole + 1 };

    explicit GlyphModel(QObject *parent = nullptr);

    void setFamily(const QString &family);
    QString family() const { return m_family; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    int rowOfCodepoint(uint cp) const; // -1 if not present
    uint codepointAt(int row) const;

private:
    QString m_family;
    QList<uint> m_cps;
};

// Glyph grid + an enlarged detail of the selected glyph.
class GlyphGrid : public QWidget {
    Q_OBJECT
public:
    explicit GlyphGrid(QWidget *parent = nullptr);

public slots:
    void setFamily(const QString &family);
    void gotoCodepoint(uint cp);

private slots:
    void onCurrentChanged(const QModelIndex &cur);

private:
    GlyphModel *m_model;
    QListView *m_view;
    QStyledItemDelegate *m_delegate; // owned by the view; concrete GlyphDelegate
    QLabel *m_detail;
    QString m_family;
};
