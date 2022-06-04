/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureaddremovewidget.hpp"

#include "../structuresmanager.hpp"
#include "../structuredefinitionfile.hpp"
#include "structureenabledlist.hpp"
#include <structureslogging.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QRegularExpression>
// Std
#include <utility>

using namespace Kasten;

namespace {

enum EntryRoles {
    OnlyOneRole = Qt::UserRole,
};
constexpr int OnlyOneRowColumn = 0;

}

StructureAddRemoveWidget::StructureAddRemoveWidget(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs,
                                                   const StructureEnabledList& enabledList,
                                                   QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* baseLayout;
    QVBoxLayout* tree1Layout;
    QVBoxLayout* tree2Layout;
    QVBoxLayout* leftRightLayout;
    QVBoxLayout* upDownLayout;

    baseLayout = new QHBoxLayout();
    baseLayout->setContentsMargins(0, 0, 0, 0);

    tree1Layout = new QVBoxLayout();
    mTree1Label = new QLabel(i18nc("@info:label", "Installed structures:"), this);
    tree1Layout->addWidget(mTree1Label);
    mTreeAvailable = new QTreeWidget(this);
    mTreeAvailable->setHeaderHidden(true);
    mTreeAvailable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeAvailable->setColumnCount(2);
    mTreeAvailable->setColumnHidden(1, true);
    tree1Layout->addWidget(mTreeAvailable);

    tree2Layout = new QVBoxLayout();
    mTree2Label = new QLabel(i18nc("@info:label", "Used structures:"), this);
    tree2Layout->addWidget(mTree2Label);
    mTreeSelected = new QTreeWidget(this);
    mTreeSelected->setHeaderHidden(true);
    mTreeSelected->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeSelected->setColumnCount(2);
    mTreeSelected->setColumnHidden(1, true);
    tree2Layout->addWidget(mTreeSelected);

    leftRightLayout = new QVBoxLayout();
    leftRightLayout->addStretch();
    mRightButton = new QPushButton(QIcon::fromTheme(QStringLiteral("arrow-right")), QString(), this);
    leftRightLayout->addWidget(mRightButton);
    mLeftButton = new QPushButton(QIcon::fromTheme(QStringLiteral("arrow-left")), QString(), this);
    leftRightLayout->addWidget(mLeftButton);
    leftRightLayout->addStretch();

    upDownLayout = new QVBoxLayout();
    upDownLayout->addStretch();
    mUpButton = new QPushButton(QIcon::fromTheme(QStringLiteral("arrow-up")), QString(), this);
    upDownLayout->addWidget(mUpButton);
    mDownButton = new QPushButton(QIcon::fromTheme(QStringLiteral("arrow-down")), QString(), this);
    upDownLayout->addWidget(mDownButton);
    upDownLayout->addStretch();

    baseLayout->addLayout(tree1Layout);
    baseLayout->addLayout(leftRightLayout);
    baseLayout->addLayout(tree2Layout);
    baseLayout->addLayout(upDownLayout);
    setLayout(baseLayout);

    connect(mLeftButton, &QPushButton::pressed, this, &StructureAddRemoveWidget::moveLeft);
    connect(mRightButton, &QPushButton::pressed, this, &StructureAddRemoveWidget::moveRight);
    connect(mUpButton, &QPushButton::pressed, this, &StructureAddRemoveWidget::moveUp);
    connect(mDownButton, &QPushButton::pressed, this, &StructureAddRemoveWidget::moveDown);

    buildAvailableList(structureDefs, enabledList);

    // already loaded defs:
    for (const StructureEnabledData& enabledData : enabledList) {
        if (enabledData.structure == QLatin1String("*")) {
            // add all of them
            StructureDefinitionFile* def = structureDefs.value(enabledData.id);
            if (!def) {
                continue;
            }
            const auto structureNames = def->structureNames();
            const bool isOnlyOne = (structureNames.size() == 1);
            for (const QString& structure : structureNames) {
                appendEnabledStructureItem(enabledData.id, structure, isOnlyOne);
            }
        } else {
            appendEnabledStructureItem(enabledData.id, enabledData.structure, false);
        }
    }
}

StructureAddRemoveWidget::~StructureAddRemoveWidget() = default;

QStringList StructureAddRemoveWidget::values() const
{
    const QString expressionTemplate = QStringLiteral("\'%1\':\'%2\'");

    QStringList enabledStructures;
    const int topLevelItemCount = mTreeSelected->topLevelItemCount();
    enabledStructures.reserve(topLevelItemCount);
    for (int i = 0; i < topLevelItemCount; ++i) {
        QTreeWidgetItem* item = mTreeSelected->topLevelItem(i);
        const bool isOnlyOne = item->data(OnlyOneRowColumn, ::OnlyOneRole).toBool();
        const QString id = item->text(1);
        const QString structure = isOnlyOne ? QStringLiteral("*") : item->text(0);
        enabledStructures.append(expressionTemplate.arg(id, structure));
    }

    return enabledStructures;
}

void StructureAddRemoveWidget::buildAvailableList(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs,
                                                  const StructureEnabledList& enabledList)
{
    QList<QTreeWidgetItem*> availableItems;
    for (StructureDefinitionFile* def : structureDefs) {
        if (!def->isValid()) {
            continue;
        }
        const QString id = def->metaData().id();
        if (!enabledList.isEnabled(id)) {
           continue;
        }
        auto* item = new QTreeWidgetItem(mTreeAvailable, QStringList { def->metaData().id(), id });
        const auto structureNames = def->structureNames();
        for (const QString& name : structureNames) {
            auto* subItem = new QTreeWidgetItem(item, QStringList { name, id });
            item->addChild(subItem);
        }

        availableItems.append(item);
    }

    mTreeAvailable->addTopLevelItems(availableItems);

}

void StructureAddRemoveWidget::appendEnabledStructureItem(const QString& id, const QString& structure,
                                                          bool isOnlyOne)
{
    auto* item = new QTreeWidgetItem(mTreeSelected, QStringList { structure, id });
    item->setData(OnlyOneRowColumn, ::OnlyOneRole, isOnlyOne);
    mTreeSelected->addTopLevelItem(item);
}

void StructureAddRemoveWidget::moveLeft()
{
    const QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    for (QTreeWidgetItem* item : selected) {
        delete mTreeSelected->takeTopLevelItem(
            mTreeSelected->indexOfTopLevelItem(item));
    }
}

void StructureAddRemoveWidget::moveRight()
{
    const QList<QTreeWidgetItem*> selected = mTreeAvailable->selectedItems();
    for (const QTreeWidgetItem* item : selected) {
        if (!item->parent()) {
            continue;     // maybe sometime add all subitems
        }
        const bool isOnlyOne = item->data(OnlyOneRowColumn, ::OnlyOneRole).toBool();
        const QString id = item->text(1);
        const QString structure = item->text(0);
        appendEnabledStructureItem(id, structure, isOnlyOne);
    }
}

void StructureAddRemoveWidget::moveUp()
{
    const QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    int firstIndex = -1;
    for (QTreeWidgetItem* item : selected) {
        int idx = mTreeSelected->indexOfTopLevelItem(item);
        int newIdx = qMax(0, idx - 1);
        mTreeSelected->insertTopLevelItem(newIdx,
                                          mTreeSelected->takeTopLevelItem(idx));
        // only first index
        firstIndex = firstIndex == -1 ? newIdx : firstIndex;
    }

    if (firstIndex != -1) {
        mTreeSelected->setCurrentItem(mTreeSelected->topLevelItem(firstIndex));
    }
}

void StructureAddRemoveWidget::moveDown()
{
    const QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    int firstIndex = -1;
    int maxItmCount = mTreeSelected->topLevelItemCount();
    for (QTreeWidgetItem* item : selected) {
        int idx = mTreeSelected->indexOfTopLevelItem(item);
        int newIdx = qMin(idx + 1, maxItmCount - 1);
        mTreeSelected->insertTopLevelItem(newIdx,
                                          mTreeSelected->takeTopLevelItem(idx));
        // only first index
        firstIndex = firstIndex == -1 ? newIdx : firstIndex;
    }

    if (firstIndex != -1) {
        mTreeSelected->setCurrentItem(mTreeSelected->topLevelItem(firstIndex));
    }
}
