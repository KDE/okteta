/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 Copyright 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

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

#include "structuresmanagerview.h"
#include "structureaddremovewidget.h"

#include "structviewpreferences.h"
#include "../structuresmanager.h"
#include "../structtool.h"
#include "../structlogging.h"
// KF5
#include <KPluginSelector>
#include <KConfigDialogManager>
#include <KPluginInfo>
#include <KLocalizedString>
#include <KNS3/Button>
// Qt
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QListWidgetItem>
#include <QLayout>
#include <QSizePolicy>


StructuresManagerView::StructuresManagerView(Kasten::StructTool* tool, QWidget* parent)
    : QWidget(parent), mTool(tool), mStructuresSelector(nullptr), mRebuildingPluginsList(false)
{
    KConfigDialogManager::changedMap()->insert(QStringLiteral("StructuresManagerView"), SIGNAL(changed(QStringList)));
    setObjectName(QStringLiteral("kcfg_LoadedStructures"));
    mSelectedStructures = Kasten::StructViewPreferences::loadedStructures();

    QVBoxLayout* pageLayout = new QVBoxLayout();
    setLayout(pageLayout);

    rebuildPluginSelectorEntries();


    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    pageLayout->addLayout(buttonsLayout);

    mGetNewStructuresButton = new KNS3::Button(i18n("Get New Structures..."),
                                               QStringLiteral("okteta-structures.knsrc"), this);
    connect(mGetNewStructuresButton, &KNS3::Button::dialogFinished,
            this, &StructuresManagerView::onGetNewStructuresClicked);
    buttonsLayout->addWidget(mGetNewStructuresButton);

    mAdvancedSelectionButton = new QPushButton(QIcon::fromTheme(QStringLiteral("configure")), i18n("Advanced Selection..."), this);
    connect(mAdvancedSelectionButton, &QPushButton::clicked, this, &StructuresManagerView::advancedSelection);
    buttonsLayout->addWidget(mAdvancedSelectionButton);
}

void StructuresManagerView::onGetNewStructuresClicked(const KNS3::Entry::List& changedEntries)
{
    foreach (const KNS3::Entry& e, changedEntries)
        {
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Changed Entry: " << e.name();
            if (e.status() == KNS3::Entry::Installed)
            {
                //new element installed
                qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed files:" << e.installedFiles();
            }
            if (e.status() == KNS3::Entry::Deleted)
            {
                //element uninstalled
                qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "deleted files:" << e.uninstalledFiles();
            }
        }
    if (!changedEntries.isEmpty())
    {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed structures changed ->  rebuilding list of installed structures";
        mTool->manager()->reloadPaths();
        rebuildPluginSelectorEntries();
    }
}

QStringList StructuresManagerView::values()
{
    return mSelectedStructures;
}


void StructuresManagerView::advancedSelection()
{
    StructureAddRemoveWidget* advancedSelectionWidget = new StructureAddRemoveWidget(mSelectedStructures, mTool, this);
    QPointer<QDialog> dlg = new QDialog(this); //the dlg-on-heap-variant
    QVBoxLayout* layout = new QVBoxLayout;
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->addButton(QDialogButtonBox::Ok);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, dlg.data(), &QDialog::accept);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, dlg.data(), &QDialog::reject);
    layout->addWidget(advancedSelectionWidget);
    layout->addWidget(dialogButtonBox);
    dlg->setLayout(layout);
    if (dlg->exec() == QDialog::Accepted) {
        QStringList newVals = advancedSelectionWidget->values();
        if (newVals != mSelectedStructures) {
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
                    << "selection changed from " << mSelectedStructures << "to" << newVals;
            mSelectedStructures = newVals;
            emit changed(newVals);
        }
    }
    delete dlg;
}

void StructuresManagerView::onPluginSelectorChange(bool change)
{
    if (mRebuildingPluginsList)
        return;
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
            << "pluginselector changed: " << change;
    if (!change)
        return;
    mStructuresSelector->save();
    reloadSelectedItems();
}

void StructuresManagerView::reloadSelectedItems() {
    QStringList newVals;
    foreach(const Kasten::StructureDefinitionFile* def, mTool->manager()->structureDefs())
    {
        KPluginInfo info = def->pluginInfo();
        if (info.isPluginEnabled())
            newVals.append(QString(QStringLiteral("\'%1\':\'*\'")).arg(info.pluginName()));
    }
    if (newVals != mSelectedStructures) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
                << "selection changed from " << mSelectedStructures << "to" << newVals;
        mSelectedStructures = newVals;
        emit changed(newVals);
    }
    else {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "no change:" << mSelectedStructures;
    }
}

void StructuresManagerView::rebuildPluginSelectorEntries()
{
    mRebuildingPluginsList = true;
    QStringList newVals;
    KPluginInfo::List plugins;
    KPluginInfo::List dynamicPlugins;
    foreach(const Kasten::StructureDefinitionFile* def, mTool->manager()->structureDefs())
        {
            KPluginInfo info = def->pluginInfo();
            if (info.category() == QLatin1String("structure"))
                plugins.append(info);
            else if (info.category() == QLatin1String("structure/js"))
                dynamicPlugins.append(info);
        }

    //XXX is there any way to clear the plugins selector?
    QBoxLayout* layoutObj = qobject_cast<QBoxLayout*>(layout());
    Q_CHECK_PTR(layoutObj);
    if (mStructuresSelector)
    {
        layoutObj->removeWidget(mStructuresSelector);
        delete mStructuresSelector;
    }
    mStructuresSelector = new KPluginSelector(this);
    connect(mStructuresSelector, &KPluginSelector::changed,
            this, &StructuresManagerView::onPluginSelectorChange);
    layoutObj->insertWidget(0, mStructuresSelector);

    mStructuresSelector->addPlugins(plugins, KPluginSelector::ReadConfigFile, i18n(
            "Structure Definitions"), QStringLiteral("structure"), mTool->manager()->config());
    mStructuresSelector->addPlugins(dynamicPlugins, KPluginSelector::ReadConfigFile,
            i18n("Dynamic Structure Definitions"), QStringLiteral("structure/js"),
            mTool->manager()->config());
    mStructuresSelector->load();
    mStructuresSelector->updatePluginsState();
    mRebuildingPluginsList = false;
    reloadSelectedItems();
}

StructuresManagerView::~StructuresManagerView()
{
}
