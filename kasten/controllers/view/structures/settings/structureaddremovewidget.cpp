/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "structureaddremovewidget.h"
#include "structviewpreferences.h"
#include "../structtool.h"
#include "../structuresmanager.h"
#include "../structuredefinitionfile.h"

#include <KPushButton>
#include <QLabel>
#include <QLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QRegExp>
#include <KLocale>
#include <KDebug>
#include <KConfigDialogManager>

using namespace Kasten2;
StructureAddRemoveWidget::StructureAddRemoveWidget(Kasten2::StructTool* tool,
        QWidget* parent) :
    QWidget(parent), mTool(tool)
{
    KConfigDialogManager::changedMap()->insert(QLatin1String("StructureAddRemoveWidget"),
            SIGNAL(changed(QStringList)));
    QHBoxLayout* baseLayout;
    QVBoxLayout* tree1Layout;
    QVBoxLayout* tree2Layout;
    QVBoxLayout* leftRightLayout;
    QVBoxLayout* upDownLayout;

    baseLayout = new QHBoxLayout();

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
    mTreeSelected->setColumnHidden(1, false);
    tree2Layout->addWidget(mTreeSelected);

    leftRightLayout = new QVBoxLayout();
    leftRightLayout->addStretch();
    mRightButton = new KPushButton(KIcon(QLatin1String("arrow-right")), QString(), this);
    leftRightLayout->addWidget(mRightButton);
    mLeftButton = new KPushButton(KIcon(QLatin1String("arrow-left")), QString(), this);
    leftRightLayout->addWidget(mLeftButton);
    leftRightLayout->addStretch();

    upDownLayout = new QVBoxLayout();
    upDownLayout->addStretch();
    mUpButton = new KPushButton(KIcon(QLatin1String("arrow-up")), QString(), this);
    upDownLayout->addWidget(mUpButton);
    mDownButton = new KPushButton(KIcon(QLatin1String("arrow-down")), QString(), this);
    upDownLayout->addWidget(mDownButton);
    upDownLayout->addStretch();

    baseLayout->addLayout(tree1Layout);
    baseLayout->addLayout(leftRightLayout);
    baseLayout->addLayout(tree2Layout);
    baseLayout->addLayout(upDownLayout);
    setLayout(baseLayout);

    connect(mLeftButton, SIGNAL(pressed()), SLOT(moveLeft()));
    connect(mRightButton, SIGNAL(pressed()), SLOT(moveRight()));
    connect(mUpButton, SIGNAL(pressed()), SLOT(moveUp()));
    connect(mDownButton, SIGNAL(pressed()), SLOT(moveDown()));

    buildAvailableList();

    //already loaded defs:
    QRegExp regex(QLatin1String("'(.+)':'(.+)'"));
    QStringList loadedStructs = StructViewPreferences::loadedStructures();
    foreach(const QString& s, loadedStructs)
        {
            int pos = regex.indexIn(s);
            if (pos > -1)
            {
                QString pluginName = regex.cap(1);
                QString structName = regex.cap(2);

                QTreeWidgetItem* item = new QTreeWidgetItem(mTreeSelected,
                        QStringList() << structName << pluginName);
                mTreeSelected->addTopLevelItem(item);
            }
        }
    syncData();
}

StructureAddRemoveWidget::~StructureAddRemoveWidget()
{
}

void StructureAddRemoveWidget::buildAvailableList()
{
    const QList<StructureDefinitionFile*> loadedDefs =
            mTool->manager()->structureDefs();
    QList<QTreeWidgetItem*> availableItems;
    foreach(StructureDefinitionFile* def,loadedDefs)
        {
            if (!def->isValid())
                continue;
            QString pluginName = def->pluginInfo().pluginName();
            if (!def->pluginInfo().isPluginEnabled())
                continue;
            QTreeWidgetItem* item = new QTreeWidgetItem(mTreeAvailable,
                    QStringList() << def->pluginInfo().pluginName() << pluginName);
            foreach(const QString& name,def->structureNames())
                {
                    QTreeWidgetItem* subItem = new QTreeWidgetItem(item,
                            QStringList() << name << pluginName);
                    item->addChild(subItem);
                }
            availableItems.append(item);
        }
    mTreeAvailable->addTopLevelItems(availableItems);

}
void StructureAddRemoveWidget::moveLeft()
{
    QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
    foreach(QTreeWidgetItem* item,selected)
        {
            delete mTreeSelected->takeTopLevelItem(
                    mTreeSelected->indexOfTopLevelItem(item));
            changed = true;
        }
    if (changed)
        syncData();
}
void StructureAddRemoveWidget::moveRight()
{
    QList<QTreeWidgetItem*> selected = mTreeAvailable->selectedItems();
    bool changed = false;
    foreach(const QTreeWidgetItem* item,selected)
        {
            if (!item->parent())
                continue; //maybe sometime add all subitems
            QTreeWidgetItem* moveOver = new QTreeWidgetItem(mTreeSelected,
                    QStringList() << item->text(0) << item->text(1));
            //item name then parent name then path
            mTreeSelected->addTopLevelItem(moveOver);
            changed = true;
        }
    if (changed)
        syncData();

}
void StructureAddRemoveWidget::moveUp()
{
    QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
    int firstIndex = -1;
    foreach(QTreeWidgetItem* item,selected)
        {
            int idx = mTreeSelected->indexOfTopLevelItem(item);
            int newIdx = qMax(0, idx - 1);
            mTreeSelected ->insertTopLevelItem(newIdx,
                    mTreeSelected->takeTopLevelItem(idx));
            //only first index
            firstIndex = firstIndex == -1 ? newIdx : firstIndex;
        }
    if (changed)
        syncData();
    if (firstIndex != -1)
        mTreeSelected->setCurrentItem(mTreeSelected->topLevelItem(firstIndex));
}
void StructureAddRemoveWidget::moveDown()
{
    QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
    int firstIndex = -1;
    int maxItmCount = mTreeSelected->topLevelItemCount();
    foreach(QTreeWidgetItem* item,selected)
        {
            int idx = mTreeSelected->indexOfTopLevelItem(item);
            int newIdx = qMin(idx + 1, maxItmCount - 1);
            mTreeSelected ->insertTopLevelItem(newIdx,
                    mTreeSelected->takeTopLevelItem(idx));
            //only first index
            firstIndex = firstIndex == -1 ? newIdx : firstIndex;
        }
    if (changed)
        syncData();
    if (firstIndex != -1)
        mTreeSelected->setCurrentItem(mTreeSelected->topLevelItem(firstIndex));
}
void StructureAddRemoveWidget::syncData()
{
    QStringList strings;
    for (int i = 0; i < mTreeSelected->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = mTreeSelected->topLevelItem(i);
        QString dataStr = QString::fromLatin1("\'%1\':\'%2\'").arg(item->text(1)).arg(
                item->text(0));
        strings.append(dataStr);
    }
    kDebug()
        << "items to set: " << strings;
    mValues = strings;
    kDebug()
        << "changed: " << mValues;
    emit changed(mValues);
}

void StructureAddRemoveWidget::updateAvailable()
{
    //rebuild available tree
    mTreeAvailable->clear();
    buildAvailableList();

    //remove any structs that references not loaded files
    QStringList plugins;
    const QList<StructureDefinitionFile*> loadedDefs =
            mTool->manager()->structureDefs();
    foreach(const StructureDefinitionFile* def, loadedDefs)
        {
            QString pluginName = def->pluginInfo().pluginName();
            if (def->pluginInfo().isValid() && !def->pluginInfo().isPluginEnabled())
                continue;
            plugins << pluginName;
        }
    bool changed = false;
    QList<QTreeWidgetItem*> toRemove;
    kDebug()
        << "paths = " << plugins;
    for (int i = 0; i < mTreeSelected->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = mTreeSelected->topLevelItem(i);
        //text(1) is plugin name
        if (!plugins.contains(item->text(1)))
        {
            kDebug()
                << "removed item: " << QString::fromLatin1("\'%1\':\'%2\'").arg(item->text(1),
                        item->text(0));

            changed = true;
            toRemove.append(item);
        }
        else
        {
            kDebug()
                << "item " << QString::fromLatin1("\'%1\':\'%2\'").arg(item->text(1),
                        item->text(0)) << "still loaded -> keep";
        }
    }
    foreach(QTreeWidgetItem* itm,toRemove)
        {
            kDebug()
                << "item " << QString::fromLatin1("\'%1\':\'%2\'").arg(itm->text(1),
                        itm->text(0)) << "removed";
            delete mTreeSelected->takeTopLevelItem(
                    mTreeSelected->indexOfTopLevelItem(itm));
        }
    if (changed)
        syncData();
}
