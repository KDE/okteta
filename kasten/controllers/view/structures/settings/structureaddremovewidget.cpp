/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#include "structureaddremovewidget.hpp"
#include "../structurestool.hpp"
#include "../structuresmanager.hpp"
#include "../structuredefinitionfile.hpp"
#include "../structlogging.hpp"

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QRegExp>
#include <KLocalizedString>
#include <KConfigDialogManager>

using namespace Kasten;

StructureAddRemoveWidget::StructureAddRemoveWidget(const QStringList& selected, Kasten::StructuresTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
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

    buildAvailableList();

    // already loaded defs:
    QRegExp regex(QStringLiteral("'(.+)':'(.+)'"));
    for (const QString& s : selected) {
        int pos = regex.indexIn(s);
        if (pos > -1) {
            QString pluginName = regex.cap(1);
            QString structName = regex.cap(2);
            if (structName == QLatin1String("*")) {
                // add all of them
                for (int i = 0; i < mTreeAvailable->topLevelItemCount(); i++) {
                    QTreeWidgetItem* avail = mTreeAvailable->topLevelItem(i);
                    if (avail->text(0) != pluginName) {
                        continue;
                    }
                    for (int i = 0; i < avail->childCount(); i++) {
                        QTreeWidgetItem* selStruct = avail->child(i);
                        QTreeWidgetItem* item = new QTreeWidgetItem(mTreeSelected,
                                                                    QStringList { selStruct->text(0), pluginName });
                        mTreeSelected->addTopLevelItem(item);
                    }

                    break;
                }
            } else {
                QTreeWidgetItem* item = new QTreeWidgetItem(mTreeSelected,
                                                            QStringList { structName, pluginName });
                mTreeSelected->addTopLevelItem(item);
            }
        }
    }

    syncData();
}

StructureAddRemoveWidget::~StructureAddRemoveWidget() = default;

QStringList StructureAddRemoveWidget::values() const
{
    return mValues;
}

void StructureAddRemoveWidget::buildAvailableList()
{
    const auto loadedDefs = mTool->manager()->structureDefs();
    QList<QTreeWidgetItem*> availableItems;
    for (StructureDefinitionFile* def : loadedDefs) {
        if (!def->isValid()) {
            continue;
        }
        QString pluginName = def->pluginInfo().pluginName();
        if (!def->pluginInfo().isPluginEnabled()) {
            continue;
        }
        QTreeWidgetItem* item = new QTreeWidgetItem(mTreeAvailable,
                                                    QStringList { def->pluginInfo().pluginName(), pluginName });
        const auto structureNames = def->structureNames();
        for (const QString& name : structureNames) {
            QTreeWidgetItem* subItem = new QTreeWidgetItem(item,
                                                           QStringList { name, pluginName });
            item->addChild(subItem);
        }

        availableItems.append(item);
    }

    mTreeAvailable->addTopLevelItems(availableItems);

}

void StructureAddRemoveWidget::moveLeft()
{
    const QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
    for (QTreeWidgetItem* item : selected) {
        delete mTreeSelected->takeTopLevelItem(
            mTreeSelected->indexOfTopLevelItem(item));
        changed = true;
    }

    if (changed) {
        syncData();
    }
}

void StructureAddRemoveWidget::moveRight()
{
    const QList<QTreeWidgetItem*> selected = mTreeAvailable->selectedItems();
    bool changed = false;
    for (const QTreeWidgetItem* item : selected) {
        if (!item->parent()) {
            continue;     // maybe sometime add all subitems
        }
        QTreeWidgetItem* moveOver = new QTreeWidgetItem(mTreeSelected,
                                                        QStringList { item->text(0), item->text(1) });
        // item name then parent name then path
        mTreeSelected->addTopLevelItem(moveOver);
        changed = true;
    }

    if (changed) {
        syncData();
    }

}

void StructureAddRemoveWidget::moveUp()
{
    const QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
    int firstIndex = -1;
    for (QTreeWidgetItem* item : selected) {
        int idx = mTreeSelected->indexOfTopLevelItem(item);
        int newIdx = qMax(0, idx - 1);
        mTreeSelected->insertTopLevelItem(newIdx,
                                          mTreeSelected->takeTopLevelItem(idx));
        // only first index
        firstIndex = firstIndex == -1 ? newIdx : firstIndex;
    }

    if (changed) {
        syncData();
    }
    if (firstIndex != -1) {
        mTreeSelected->setCurrentItem(mTreeSelected->topLevelItem(firstIndex));
    }
}

void StructureAddRemoveWidget::moveDown()
{
    const QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
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

    if (changed) {
        syncData();
    }
    if (firstIndex != -1) {
        mTreeSelected->setCurrentItem(mTreeSelected->topLevelItem(firstIndex));
    }
}
void StructureAddRemoveWidget::syncData()
{
    const auto topLevelItemCount = mTreeSelected->topLevelItemCount();
    QStringList strings;
    strings.reserve(topLevelItemCount);
    for (int i = 0; i < topLevelItemCount; ++i) {
        QTreeWidgetItem* item = mTreeSelected->topLevelItem(i);
        QString dataStr = QStringLiteral("\'%1\':\'%2\'").arg(item->text(1), item->text(0));
        strings.append(dataStr);
    }

    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "selection changed to: " << strings;
    mValues = strings;
}

void StructureAddRemoveWidget::updateAvailable()
{
    // rebuild available tree
    mTreeAvailable->clear();
    buildAvailableList();

    // remove any structs that references not loaded files
    QStringList plugins;
    const auto loadedDefs = mTool->manager()->structureDefs();
    for (const StructureDefinitionFile* def : loadedDefs) {
        QString pluginName = def->pluginInfo().pluginName();
        if (def->pluginInfo().isValid() && !def->pluginInfo().isPluginEnabled()) {
            continue;
        }
        plugins << pluginName;
    }

    bool changed = false;
    QList<QTreeWidgetItem*> toRemove;
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "paths = " << plugins;
    for (int i = 0; i < mTreeSelected->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = mTreeSelected->topLevelItem(i);
        // text(1) is plugin name
        if (!plugins.contains(item->text(1))) {
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
                << "removed item: " << QStringLiteral("\'%1\':\'%2\'").arg(item->text(1),
                                                                           item->text(0));

            changed = true;
            toRemove.append(item);
        } else {
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
                << "item " << QStringLiteral("\'%1\':\'%2\'").arg(item->text(1),
                                                                  item->text(0)) << "still loaded -> keep";
        }
    }

    for (QTreeWidgetItem* itm : qAsConst(toRemove)) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
            << "item " << QStringLiteral("\'%1\':\'%2\'").arg(itm->text(1),
                                                              itm->text(0)) << "removed";
        delete mTreeSelected->takeTopLevelItem(mTreeSelected->indexOfTopLevelItem(itm));
    }

    if (changed) {
        syncData();
    }
}
