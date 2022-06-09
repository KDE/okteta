/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytetableview.hpp"

// tool
#include "bytetabletool.hpp"
#include "bytetablemodel.hpp"
#include <bytetableviewsettings.hpp>
// utils
#include <labelledtoolbarwidget.hpp>
// KF
#include <KLocalizedString>
#include <KStandardGuiItem>
// Qt
#include <QApplication>
#include <QToolBar>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTreeView>
#include <QAction>
#include <QIcon>
// Std
#include <limits>

namespace Kasten {

// TODO: move config store/restore to tool
static constexpr int DefaultInsertCount = 1;

static constexpr char ByteTableConfigGroupId[] = "ByteTableTool";
static constexpr char InsertCountConfigKey[] = "InsertCount";

ByteTableView::ByteTableView(ByteTableTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    mByteTableView = new QTreeView(this);
    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteTableView::setFixedFontByGlobalSettings );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteTableView::resizeColumnsWidth );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayStyleChanged,
//              this, &ByteTableView::resizeColumnsWidth );
    mByteTableView->setObjectName(QStringLiteral("ByteTable"));
    mByteTableView->setRootIsDecorated(false);
    mByteTableView->setItemsExpandable(false);
    mByteTableView->setUniformRowHeights(true);
    mByteTableView->setAllColumnsShowFocus(true);
    mByteTableView->setSortingEnabled(false);
    QHeaderView* header = mByteTableView->header();
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(false);
    mByteTableView->setModel(mTool->byteTableModel());
    connect(mByteTableView, &QTreeView::doubleClicked,
            this, &ByteTableView::onDoubleClicked);

    baseLayout->addWidget(mByteTableView, 10);

    auto* insertToolBar = new QToolBar(this);
    insertToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    insertToolBar->addWidget(stretcher);

    auto* label = new QLabel(i18nc("@label:spinbox number of bytes to insert", "Number (bytes):"), this);

    mInsertCountEdit = new QSpinBox(this);
    mInsertCountEdit->setRange(1, std::numeric_limits<int>::max());
    auto* labelledInsertCountEdit = new LabelledToolBarWidget(label, mInsertCountEdit, this);
    insertToolBar->addWidget(labelledInsertCountEdit);
    const QString insertCountToolTip =
        i18nc("@info:tooltip",
              "Number of repeats of the currently selected byte in the table to be inserted.");
    label->setToolTip(insertCountToolTip);
    mInsertCountEdit->setToolTip(insertCountToolTip);
    connect(mInsertCountEdit, qOverload<int>(&QSpinBox::valueChanged),
            this, &ByteTableView::onInsertCountEditChanged);

    const KGuiItem insertGuiItem = KStandardGuiItem::insert();
    QIcon insertIcon = insertGuiItem.icon();
    if (insertIcon.isNull()) {
        insertIcon = QIcon::fromTheme(QStringLiteral("list-add"));
    }
    mInsertAction =
        insertToolBar->addAction(insertIcon, insertGuiItem.text(),
                                 this, &ByteTableView::onInsertClicked);
    const QString insertButtonToolTip =
        i18nc("@info:tooltip",
              "Insert the currently selected byte in the table repeated the given number of times.");
    mInsertAction->setToolTip(insertButtonToolTip);
    mInsertAction->setEnabled(mTool->hasWriteable());
    connect(mTool, &ByteTableTool::hasWriteableChanged, mInsertAction, &QAction::setEnabled);

    baseLayout->addWidget(insertToolBar);

    // if nothing has changed reuse the old values. This means the bytetable is fully constructed
    // after ~3ms and not 800 as it was before. If the saved values can not be reused it takes ~100ms
    const QList<int> columnsWidth = ByteTableViewSettings::columnsWidth();
    const QString styleName = QApplication::style()->objectName();
    const QString fontData = mByteTableView->font().toString();
    const QString fixedFontData = mTool->byteTableModel()->fixedFont().toString();
    if ((ByteTableModel::NoOfIds > columnsWidth.size()) ||
        (styleName != ByteTableViewSettings::style()) ||
        (fontData != ByteTableViewSettings::font()) ||
        (fixedFontData != ByteTableViewSettings::fixedFont())) {
        resizeColumnsWidth();
    } else {
        for (int i = 0; i < ByteTableModel::NoOfIds; ++i) {
            header->resizeSection(i, columnsWidth.at(i));
        }
    }

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ByteTableConfigGroupId);
    const int insertCount = configGroup.readEntry(InsertCountConfigKey, DefaultInsertCount);

    mInsertCountEdit->setValue(insertCount);

}

ByteTableView::~ByteTableView()
{
    QList<int> columnsWidth;
    columnsWidth.reserve(ByteTableModel::NoOfIds);
    const QHeaderView* header = mByteTableView->header();
    for (int i = 0; i < ByteTableModel::NoOfIds; ++i) {
        columnsWidth.append(header->sectionSize(i));
    }

    ByteTableViewSettings::setColumnsWidth(columnsWidth);
    ByteTableViewSettings::setStyle(QApplication::style()->objectName());
    ByteTableViewSettings::setFont(mByteTableView->font().toString());
    ByteTableViewSettings::setFixedFont(mTool->byteTableModel()->fixedFont().toString());
    ByteTableViewSettings::self()->save();
}

void ByteTableView::resizeColumnsWidth()
{
    QHeaderView* header = mByteTableView->header();
    for (int i = 0; i < ByteTableModel::NoOfIds; ++i) {
        if (i == ByteTableModel::CharacterId) {
            mByteTableView->resizeColumnToContents(i);
            continue;
        }
        // since all indexes in one row have same number of chars it is enough to calculate one row
        // this speeds up calculating the width from 800ms to 100ms
        const QModelIndex index = mTool->byteTableModel()->index(0, i);
        const int indexWidthHint = mByteTableView->sizeHintForIndex(index).width();
        const int headerWidthHint = header->sectionSizeHint(i);
        header->resizeSection(i, qMax(indexWidthHint, headerWidthHint));
    }
}

void ByteTableView::onInsertCountEditChanged()
{
    KConfigGroup configGroup(KSharedConfig::openConfig(), ByteTableConfigGroupId);
    configGroup.writeEntry(InsertCountConfigKey, mInsertCountEdit->value());
}

void ByteTableView::onDoubleClicked(const QModelIndex& index)
{
    if (!mTool->hasWriteable()) {
        return;
    }

    const unsigned char byte = index.row();
    mTool->insert(byte, mInsertCountEdit->value());
}

void ByteTableView::onInsertClicked()
{
    const unsigned char byte = mByteTableView->currentIndex().row();
    mTool->insert(byte, mInsertCountEdit->value());
}

}
