// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Madalin Ignisca <git@madalin.me>

#include "mainwindow.h"
#include "fontfilterproxy.h"
#include "previewwidget.h"
#include "glyphgrid.h"

#include <QListView>
#include <QStringListModel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFontDatabase>
#include <QStatusBar>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(buildFontPanel());

    m_tabs = new QTabWidget(this);
    m_tabs->addTab(buildPreviewTab(), tr("Preview"));
    m_tabs->addTab(buildGlyphTab(), tr("Glyphs"));
    splitter->addWidget(m_tabs);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({ 260, 760 });
    setCentralWidget(splitter);
    statusBar();

    // Select the first family so the UI is populated on launch.
    if (m_proxy->rowCount() > 0) {
        const QModelIndex first = m_proxy->index(0, 0);
        m_fontList->setCurrentIndex(first);
        onFontSelected(first.data(Qt::DisplayRole).toString());
    }
    applyParams();

    setWindowTitle(tr("Qt Font Viewer"));
    resize(1040, 700);
}

QWidget *MainWindow::buildFontPanel()
{
    auto *panel = new QWidget(this);
    auto *lay = new QVBoxLayout(panel);

    m_search = new QLineEdit(panel);
    m_search->setPlaceholderText(tr("Filter families…"));
    m_search->setClearButtonEnabled(true);

    m_monoOnly = new QCheckBox(tr("Monospace only"), panel);

    m_fontModel = new QStringListModel(QFontDatabase::families(), this);
    m_proxy = new FontFilterProxy(this);
    m_proxy->setSourceModel(m_fontModel);
    m_proxy->sort(0);

    m_fontList = new QListView(panel);
    m_fontList->setModel(m_proxy);
    m_fontList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fontList->setSelectionMode(QAbstractItemView::SingleSelection);

    lay->addWidget(m_search);
    lay->addWidget(m_monoOnly);
    lay->addWidget(m_fontList, 1);

    connect(m_search, &QLineEdit::textChanged, m_proxy, &FontFilterProxy::setNameFilter);
    connect(m_monoOnly, &QCheckBox::toggled, m_proxy, &FontFilterProxy::setMonospaceOnly);
    connect(m_fontList->selectionModel(), &QItemSelectionModel::currentChanged, this,
        [this](const QModelIndex &cur, const QModelIndex &) {
            if (cur.isValid())
                onFontSelected(cur.data(Qt::DisplayRole).toString());
        });
    return panel;
}

QWidget *MainWindow::buildPreviewTab()
{
    auto *tab = new QWidget(this);
    auto *lay = new QVBoxLayout(tab);

    // --- controls row ---
    auto *controls = new QHBoxLayout;
    m_sample = new QLineEdit(tab);
    m_sample->setText(QStringLiteral("The quick brown fox 0123 _____ |il1 oO0 g_y"));

    m_size = new QSpinBox(tab);
    m_size->setRange(4, 200);
    m_size->setValue(16);
    m_size->setPrefix(tr("px "));

    m_zoom = new QSpinBox(tab);
    m_zoom->setRange(1, 24);
    m_zoom->setValue(6);
    m_zoom->setPrefix(tr("×"));

    m_aa = new QCheckBox(tr("Antialias"), tab);
    m_aa->setChecked(false); // start in console mode
    m_hint = new QCheckBox(tr("Hinting"), tab);
    m_hint->setChecked(true);
    m_compare = new QCheckBox(tr("Compare"), tab);

    controls->addWidget(new QLabel(tr("Text:"), tab));
    controls->addWidget(m_sample, 1);
    controls->addWidget(m_size);
    controls->addWidget(new QLabel(tr("Zoom:"), tab));
    controls->addWidget(m_zoom);
    controls->addWidget(m_aa);
    controls->addWidget(m_hint);
    controls->addWidget(m_compare);
    lay->addLayout(controls);

    // --- preview area (pane A, optional pane B) ---
    m_prevA = new PreviewWidget(tab);
    auto *scrollA = new QScrollArea(tab);
    scrollA->setWidget(m_prevA);
    scrollA->setWidgetResizable(true);
    scrollA->setBackgroundRole(QPalette::Base);

    // pane B container
    m_paneB = new QWidget(tab);
    auto *bLay = new QVBoxLayout(m_paneB);
    bLay->setContentsMargins(0, 0, 0, 0);
    m_familyB = new QComboBox(m_paneB);
    m_familyB->addItems(QFontDatabase::families());
    m_prevB = new PreviewWidget(m_paneB);
    auto *scrollB = new QScrollArea(m_paneB);
    scrollB->setWidget(m_prevB);
    scrollB->setWidgetResizable(true);
    scrollB->setBackgroundRole(QPalette::Base);
    bLay->addWidget(m_familyB);
    bLay->addWidget(scrollB, 1);
    m_paneB->setVisible(false);

    auto *panes = new QHBoxLayout;
    panes->addWidget(scrollA, 1);
    panes->addWidget(m_paneB, 1);
    lay->addLayout(panes, 1);

    // --- clipping readout ---
    m_clip = new QLabel(tab);
    m_clip->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_clip->setStyleSheet(QStringLiteral("QLabel { font-family: monospace; padding: 4px; }"));
    auto *legend
        = new QLabel(tr("overlay:  green = cell top   blue = baseline   red = cell bottom"), tab);
    legend->setStyleSheet(QStringLiteral("QLabel { color: gray; }"));
    lay->addWidget(legend);
    lay->addWidget(m_clip);

    // --- wiring ---
    connect(m_sample, &QLineEdit::textChanged, this, &MainWindow::applyParams);
    connect(m_size, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::applyParams);
    connect(m_zoom, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::applyParams);
    connect(m_aa, &QCheckBox::toggled, this, &MainWindow::applyParams);
    connect(m_hint, &QCheckBox::toggled, this, &MainWindow::applyParams);
    connect(m_compare, &QCheckBox::toggled, this, &MainWindow::onCompareToggled);
    connect(m_familyB, &QComboBox::currentTextChanged, this, [this](const QString &fam) {
        if (m_prevB)
            m_prevB->setFamily(fam);
    });
    connect(m_prevA, &PreviewWidget::metricsUpdated, this,
        [this](const QString &r) { m_clip->setText(r); });

    // zoom only matters in console mode
    connect(m_aa, &QCheckBox::toggled, m_zoom, [this](bool aa) { m_zoom->setEnabled(!aa); });
    return tab;
}

QWidget *MainWindow::buildGlyphTab()
{
    auto *tab = new QWidget(this);
    auto *lay = new QVBoxLayout(tab);

    auto *row = new QHBoxLayout;
    m_goto = new QLineEdit(tab);
    m_goto->setPlaceholderText(tr("Go to codepoint, e.g. 5F or U+2500"));
    auto *go = new QPushButton(tr("Go"), tab);
    row->addWidget(new QLabel(tr("Jump:"), tab));
    row->addWidget(m_goto, 1);
    row->addWidget(go);
    lay->addLayout(row);

    m_glyphs = new GlyphGrid(tab);
    lay->addWidget(m_glyphs, 1);

    connect(go, &QPushButton::clicked, this, &MainWindow::onGoto);
    connect(m_goto, &QLineEdit::returnPressed, this, &MainWindow::onGoto);
    return tab;
}

void MainWindow::onFontSelected(const QString &family)
{
    m_currentFamily = family;
    if (m_prevA)
        m_prevA->setFamily(family);
    if (m_glyphs)
        m_glyphs->setFamily(family);
    refreshStatus(family);
}

void MainWindow::applyParams()
{
    const QString text = m_sample->text();
    const int px = m_size->value();
    const int z = m_zoom->value();
    const bool aa = m_aa->isChecked();
    const bool hint = m_hint->isChecked();

    for (PreviewWidget *pw : { m_prevA, m_prevB }) {
        if (!pw)
            continue;
        pw->setText(text);
        pw->setPixelSize(px);
        pw->setZoom(z);
        pw->setAntialias(aa);
        pw->setHinting(hint);
    }
    if (m_prevA)
        m_prevA->setFamily(m_currentFamily);
}

void MainWindow::onCompareToggled(bool on)
{
    m_paneB->setVisible(on);
    if (on && m_prevB) {
        if (m_familyB->currentText().isEmpty())
            m_familyB->setCurrentText(m_currentFamily);
        m_prevB->setFamily(m_familyB->currentText());
        applyParams();
    }
}

void MainWindow::onGoto()
{
    QString s = m_goto->text().trimmed();
    s.remove(QStringLiteral("U+"), Qt::CaseInsensitive);
    bool ok = false;
    const uint cp = s.toUInt(&ok, 16);
    if (ok && m_glyphs)
        m_glyphs->gotoCodepoint(cp);
}

void MainWindow::refreshStatus(const QString &family)
{
    const QStringList styles = QFontDatabase::styles(family);
    const bool mono = QFontDatabase::isFixedPitch(family);
    const bool scalable = QFontDatabase::isSmoothlyScalable(family);
    statusBar()->showMessage(tr("%1   •   %2 style(s)   •   monospace: %3   •   scalable: %4")
            .arg(family)
            .arg(styles.size())
            .arg(mono ? tr("yes") : tr("no"))
            .arg(scalable ? tr("yes") : tr("no")));
}
