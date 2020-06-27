/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infoview.hpp"

// tool
#include "infotool.hpp"
#include "statistictablemodel.hpp"
#include <infoviewsettings.h>
// KF
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QSortFilterProxyModel>
#include <QFontDatabase>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QHeaderView>
#include <QTreeView>
#include <QFontMetrics>

namespace Kasten {

InfoView::InfoView(InfoTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    auto* topLineLayout = new QHBoxLayout();

    QLabel* label = new QLabel(i18nc("@label size of selected bytes", "Size:"), this);
    topLineLayout->addWidget(label);

    mSizeLabel = new QLabel(this);
    const QString sizeToolTip =
        i18nc("@info:tooltip",
              "The number of the bytes the statistic was built for.");
    label->setToolTip(sizeToolTip);
    mSizeLabel->setToolTip(sizeToolTip);
    topLineLayout->addWidget(mSizeLabel, 10);
    connect(mTool->statisticTableModel(), &StatisticTableModel::sizeChanged,
            this, &InfoView::setByteArraySize);

    topLineLayout->addStretch();

    const KGuiItem updateGuiItem =
        KGuiItem(i18nc("@action:button build the statistic of the byte frequency",
                       "&Build"),
                 QStringLiteral("run-build"),
                 i18nc("@info:tooltip",
                       "Builds the byte frequency statistic for the bytes in the selected range."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Build</interface> button,"
                        " the byte frequency statistic is built for the bytes in the selected range."));
    mUpdateButton = new QPushButton(this);
    KGuiItem::assign(mUpdateButton, updateGuiItem);
    mUpdateButton->setEnabled(mTool->isApplyable());
    connect(mTool, &InfoTool::isApplyableChanged, mUpdateButton, &QPushButton::setEnabled);
    connect(mUpdateButton, &QPushButton::clicked, mTool, &InfoTool::updateStatistic);
    topLineLayout->addWidget(mUpdateButton);

    baseLayout->addLayout(topLineLayout);

    mStatisticTableView = new QTreeView(this);
    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &InfoView::setFixedFontByGlobalSettings );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &InfoView::resizeColumnsWidth );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayStyleChanged,
//              this, &InfoView::resizeColumnsWidth );
    setFixedFontByGlobalSettings(); // do this before setting model
    mStatisticTableView->setObjectName(QStringLiteral("StatisticTable"));
    mStatisticTableView->setRootIsDecorated(false);
    mStatisticTableView->setItemsExpandable(false);
    mStatisticTableView->setUniformRowHeights(true);
    mStatisticTableView->setAllColumnsShowFocus(true);
    mStatisticTableView->setSortingEnabled(true);
    QHeaderView* header = mStatisticTableView->header();
    header->setFont(font());
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(false);
    // TODO: write subclass to filter count and percent by num, not string
    auto* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(mTool->statisticTableModel());
    mStatisticTableView->setModel(proxyModel);
    mStatisticTableView->sortByColumn(StatisticTableModel::CountId, Qt::DescendingOrder);
    connect(mTool->statisticTableModel(), &StatisticTableModel::headerChanged, this, &InfoView::updateHeader);

    baseLayout->addWidget(mStatisticTableView, 10);

    setByteArraySize(mTool->size());

    // if nothing has changed reuse the old values. This means the info view is fully constructed much quicker.
    const QList<int> columnsWidth = InfoViewSettings::columnsWidth();
    const QString styleName = QApplication::style()->objectName();
    const QString fixedFontData = QFontDatabase::systemFont(QFontDatabase::FixedFont).toString();
    if (columnsWidth.size() < StatisticTableModel::NoOfIds || styleName != InfoViewSettings::style()
        || fixedFontData != InfoViewSettings::fixedFont()) {
        resizeColumnsWidth();
    } else {
        for (int i = 0; i < StatisticTableModel::NoOfIds; ++i) {
            header->resizeSection(i, columnsWidth.at(i));
        }
    }
}

InfoView::~InfoView()
{
    QList<int> columnsWidth;
    columnsWidth.reserve(StatisticTableModel::NoOfIds);
    const QHeaderView* header = mStatisticTableView->header();
    for (int i = 0; i < StatisticTableModel::NoOfIds; ++i) {
        columnsWidth.append(header->sectionSize(i));
    }

    InfoViewSettings::setColumnsWidth(columnsWidth);
    InfoViewSettings::setStyle(QApplication::style()->objectName());
    InfoViewSettings::setFixedFont(QFontDatabase::systemFont(QFontDatabase::FixedFont).toString());
    InfoViewSettings::self()->save();
}

void InfoView::updateHeader()
{
    mStatisticTableView->resizeColumnToContents(StatisticTableModel::ValueId);
    mStatisticTableView->header()->headerDataChanged(Qt::Horizontal,
                                                     StatisticTableModel::ValueId, StatisticTableModel::ValueId);
}

void InfoView::resizeColumnsWidth()
{
    for (int i = 0; i < StatisticTableModel::NoOfIds; ++i) {
        mStatisticTableView->resizeColumnToContents(i);
    }
}

void InfoView::setByteArraySize(int size)
{
    const QString sizeText = (size < 1) ?   // -1 is default, 0 should not happen
                             QStringLiteral("-") :
                             i18np("1 byte", "%1 bytes", size);

    mSizeLabel->setText(sizeText);
}

void InfoView::setFixedFontByGlobalSettings()
{
    mStatisticTableView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

}
