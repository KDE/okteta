/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infoview.hpp"

// tool
#include "infotool.hpp"
#include "statistictablemodel.hpp"
#include <infoviewsettings.hpp>
// utils
#include <labelledtoolbarwidget.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QClipboard>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTreeView>
#include <QAction>
#include <QIcon>

namespace Kasten {

InfoView::InfoView(InfoTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    auto* buildToolBar = new QToolBar(this);
    buildToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    auto* label = new QLabel(i18nc("@label size of selected bytes", "Size:"), this);

    mSizeLabel = new QLabel(this);
    const QString sizeToolTip =
        i18nc("@info:tooltip",
              "The number of the bytes the statistic was built for.");
    label->setToolTip(sizeToolTip);
    mSizeLabel->setToolTip(sizeToolTip);
    auto* labelledSizeLabel = new LabelledToolBarWidget(label, mSizeLabel, this);
    buildToolBar->addWidget(labelledSizeLabel);
    connect(mTool->statisticTableModel(), &StatisticTableModel::sizeChanged,
            this, &InfoView::setByteArraySize);

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    buildToolBar->addWidget(stretcher);

    mUpdateAction =
        buildToolBar->addAction(QIcon::fromTheme(QStringLiteral("run-build")),
                                i18nc("@action:button build the statistic of the byte frequency",
                                      "&Build"),
                                mTool, &InfoTool::updateStatistic);
    mUpdateAction->setToolTip(i18nc("@info:tooltip",
                                    "Builds the byte frequency statistic for the bytes in the selected range."));
    mUpdateAction->setWhatsThis(xi18nc("@info:whatsthis",
                                       "If you press the <interface>Build</interface> button,"
                                       " the byte frequency statistic is built for the bytes in the selected range."));
    mUpdateAction->setEnabled(mTool->isApplyable());
    connect(mTool, &InfoTool::isApplyableChanged, mUpdateAction, &QAction::setEnabled);

    baseLayout->addWidget(buildToolBar);

    mStatisticTableView = new QTreeView(this);
    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &InfoView::setFixedFontByGlobalSettings );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &InfoView::resizeColumnsWidth );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayStyleChanged,
//              this, &InfoView::resizeColumnsWidth );
    mStatisticTableView->setObjectName(QStringLiteral("StatisticTable"));
    mStatisticTableView->setRootIsDecorated(false);
    mStatisticTableView->setItemsExpandable(false);
    mStatisticTableView->setUniformRowHeights(true);
    mStatisticTableView->setAllColumnsShowFocus(true);
    mStatisticTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mStatisticTableView->setSortingEnabled(true);
    QHeaderView* header = mStatisticTableView->header();
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(false);
    auto* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSortRole(StatisticTableModel::SortRole);
    proxyModel->setSourceModel(mTool->statisticTableModel());
    mStatisticTableView->setModel(proxyModel);
    mStatisticTableView->sortByColumn(StatisticTableModel::CountId, Qt::DescendingOrder);
    connect(mTool->statisticTableModel(), &StatisticTableModel::headerChanged, this, &InfoView::updateHeader);
    connect(mStatisticTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &InfoView::onTableSelectionChanged);

    baseLayout->addWidget(mStatisticTableView, 10);

    // actions
    auto* actionsToolBar = new QToolBar(this);
    actionsToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    mCopyAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                  i18n("C&opy"),
                                  this, &InfoView::onCopyButtonClicked);
    mCopyAction->setToolTip(i18nc("@info:tooltip",
                                  "Copies the selected statistic lines to the clipboard."));
    mCopyAction->setWhatsThis(xi18nc("@info:whatsthis",
                                     "If you press the <interface>Copy</interface> button, all statistic lines you selected "
                                     "in the list are copied to the clipboard."));

    actionsToolBar->addAction(mCopyAction);

    auto* actionsStretcher = new QWidget(this);
    actionsStretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(actionsStretcher);

    baseLayout->addWidget(actionsToolBar);

    // init
    setByteArraySize(mTool->size());
    onTableSelectionChanged();

    // if nothing has changed reuse the old values. This means the info view is fully constructed much quicker.
    const QList<int> columnsWidth = InfoViewSettings::columnsWidth();
    const QString styleName = QApplication::style()->objectName();
    const QString fontData = mStatisticTableView->font().toString();
    const QString fixedFontData = mTool->statisticTableModel()->fixedFont().toString();
    if ((StatisticTableModel::NoOfIds > columnsWidth.size()) ||
        (styleName != InfoViewSettings::style()) ||
        (fontData != InfoViewSettings::font()) ||
        (fixedFontData != InfoViewSettings::fixedFont())) {
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
    InfoViewSettings::setFont(mStatisticTableView->font().toString());
    InfoViewSettings::setFixedFont(mTool->statisticTableModel()->fixedFont().toString());
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

void InfoView::onCopyButtonClicked()
{
    QString lines;

    const QModelIndexList rows = mStatisticTableView->selectionModel()->selectedRows();
    for (const QModelIndex& rowIndex : rows) {
        for (int column = 0; column < StatisticTableModel::NoOfIds; ++column) {
            lines += rowIndex.siblingAtColumn(column).data(Qt::DisplayRole).toString();
            // add tab, but for last linefeed
            lines += (column < StatisticTableModel::NoOfIds - 1) ? QLatin1Char('\t') :
            QLatin1Char('\n'); // TODO: specific linefeed for platforms
        }
    }

    QApplication::clipboard()->setText(lines);
}

void InfoView::onTableSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mStatisticTableView->selectionModel();

    // TODO: selectionModel->selectedIndexes() is a expensive operation,
    // but with Qt 4.4.3 hasSelection() has the flaw to return true with a current index
    const bool hasSelection = !selectionModel->selectedIndexes().isEmpty();
    mCopyAction->setEnabled(hasSelection);
}


}

#include "moc_infoview.cpp"
