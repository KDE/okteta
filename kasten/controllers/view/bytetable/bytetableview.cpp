/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>
    Copyright 2011 Alex Richardson <alex.richardson@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "bytetableview.hpp"

// tool
#include "bytetabletool.hpp"
#include "bytetablemodel.hpp"
#include <bytetableviewsettings.h>
// KF5
#include <QPushButton>
#include <KLocalizedString>
#include <KStandardGuiItem>
// Qt
#include <QApplication>
#include <QSpinBox>
#include <QFontDatabase>
#include <QLabel>
#include <QLayout>
#include <QHeaderView>
#include <QTreeView>

namespace Kasten {

ByteTableView::ByteTableView(ByteTableTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    QVBoxLayout* baseLayout = new QVBoxLayout(this);
    baseLayout->setMargin(0);

    mByteTableView = new QTreeView(this);
    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteTableView::setFixedFontByGlobalSettings );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &ByteTableView::resizeColumnsWidth );
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayStyleChanged,
//              this, &ByteTableView::resizeColumnsWidth );
    setFixedFontByGlobalSettings(); // do this before setting model
    mByteTableView->setObjectName(QStringLiteral("ByteTable"));
    mByteTableView->setRootIsDecorated(false);
    mByteTableView->setItemsExpandable(false);
    mByteTableView->setUniformRowHeights(true);
    mByteTableView->setAllColumnsShowFocus(true);
    mByteTableView->setSortingEnabled(false);
    QHeaderView* header = mByteTableView->header();
    header->setFont(font());
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(false);
    mByteTableView->setModel(mTool->byteTableModel());
    connect(mByteTableView, &QTreeView::doubleClicked,
            this, &ByteTableView::onDoubleClicked);

    baseLayout->addWidget(mByteTableView, 10);

    QHBoxLayout* insertLayout = new QHBoxLayout();

    QLabel* label = new QLabel(i18nc("@label:spinbox number of bytes to insert", "Number (bytes):"), this);
    insertLayout->addWidget(label);

    mInsertCountEdit = new QSpinBox(this);
    mInsertCountEdit->setRange(1, INT_MAX);
    mInsertCountEdit->setValue(1);
    label->setBuddy(mInsertCountEdit);
    insertLayout->addWidget(mInsertCountEdit);
    const QString insertCountToolTip =
        i18nc("@info:tooltip",
              "Number of repeats of the currently selected byte in the table to be inserted.");
    label->setToolTip(insertCountToolTip);
    mInsertCountEdit->setToolTip(insertCountToolTip);

    insertLayout->addStretch();

    mInsertButton = new QPushButton(this);
    KGuiItem::assign(mInsertButton, KStandardGuiItem::insert());
    mInsertButton->setEnabled(mTool->hasWriteable());
    connect(mTool, &ByteTableTool::hasWriteableChanged, mInsertButton, &QPushButton::setEnabled);
    connect(mInsertButton, &QPushButton::clicked, this, &ByteTableView::onInsertClicked);
    const QString insertButtonToolTip =
        i18nc("@info:tooltip",
              "Insert the currently selected byte in the table repeated the given number of times.");
    mInsertButton->setToolTip(insertButtonToolTip);
    addButton(mInsertButton, AbstractToolWidget::Default);
    insertLayout->addWidget(mInsertButton);

    baseLayout->addLayout(insertLayout);

    // if nothing has changed reuse the old values. This means the bytetable is fully constructed
    // after ~3ms and not 800 as it was before. If the saved values can not be reused it takes ~100ms
    const QList<int> columnsWidth = ByteTableViewSettings::columnsWidth();
    const QString styleName = QApplication::style()->objectName();
    const QString fixedFontData = QFontDatabase::systemFont(QFontDatabase::FixedFont).toString();
    if (columnsWidth.size() < ByteTableModel::NoOfIds || styleName != ByteTableViewSettings::style()
        || fixedFontData != ByteTableViewSettings::fixedFont()) {
        resizeColumnsWidth();
    } else {
        for (int i = 0; i < ByteTableModel::NoOfIds; ++i) {
            header->resizeSection(i, columnsWidth.at(i));
        }
    }
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
    ByteTableViewSettings::setFixedFont(QFontDatabase::systemFont(QFontDatabase::FixedFont).toString());
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

void ByteTableView::setFixedFontByGlobalSettings()
{
    mByteTableView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
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
