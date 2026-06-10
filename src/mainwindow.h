// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#pragma once
#include <QMainWindow>
#include <QString>

class QListView;
class QStringListModel;
class QLineEdit;
class QCheckBox;
class QSpinBox;
class QTabWidget;
class QComboBox;
class QLabel;
class QWidget;

class FontFilterProxy;
class PreviewWidget;
class GlyphGrid;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onFontSelected(const QString &family);
    void applyParams();              // push text/size/zoom/aa/hint to both previews
    void onCompareToggled(bool on);
    void onGoto();

private:
    QWidget *buildFontPanel();
    QWidget *buildPreviewTab();
    QWidget *buildGlyphTab();
    void     refreshStatus(const QString &family);

    // font list
    QStringListModel *m_fontModel = nullptr;
    FontFilterProxy  *m_proxy     = nullptr;
    QListView        *m_fontList  = nullptr;
    QLineEdit        *m_search    = nullptr;
    QCheckBox        *m_monoOnly  = nullptr;

    QTabWidget       *m_tabs      = nullptr;

    // preview controls
    QLineEdit     *m_sample  = nullptr;
    QSpinBox      *m_size    = nullptr;
    QSpinBox      *m_zoom    = nullptr;
    QCheckBox     *m_aa      = nullptr;
    QCheckBox     *m_hint    = nullptr;
    QCheckBox     *m_compare = nullptr;
    QLabel        *m_clip    = nullptr;
    PreviewWidget *m_prevA   = nullptr;
    PreviewWidget *m_prevB   = nullptr;
    QWidget       *m_paneB   = nullptr;
    QComboBox     *m_familyB = nullptr;

    // glyphs
    GlyphGrid *m_glyphs = nullptr;
    QLineEdit *m_goto   = nullptr;

    QString m_currentFamily;
};
