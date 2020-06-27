/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

 SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "structuresmanagerview.hpp"

// controller
#include "structuresselectiondialog.hpp"
#include "structureviewpreferences.h"
#include "../structuresmanager.hpp"
#include "../structurestool.hpp"
#include "../structlogging.hpp"
// KF
#include <KPluginSelector>
#include <KConfigDialogManager>
#include <KPluginInfo>
#include <KLocalizedString>
#include <KNS3/Button>
// Qt
#include <QPushButton>
#include <QListWidgetItem>
#include <QLayout>
#include <QSizePolicy>

StructuresManagerView::StructuresManagerView(Kasten::StructuresTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
    , mRebuildingPluginsList(false)
{
    mSelectedStructures = Kasten::StructureViewPreferences::loadedStructures();

    auto* pageLayout = new QVBoxLayout();
    pageLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pageLayout);

    rebuildPluginSelectorEntries();

    auto* buttonsLayout = new QHBoxLayout();
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

StructuresManagerView::~StructuresManagerView() = default;

void StructuresManagerView::onGetNewStructuresClicked(const KNS3::Entry::List& changedEntries)
{
    for (const KNS3::Entry& e : changedEntries) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Changed Entry: " << e.name();
        if (e.status() == KNS3::Entry::Installed) {
            // new element installed
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed files:" << e.installedFiles();
        }
        if (e.status() == KNS3::Entry::Deleted) {
            // element uninstalled
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "deleted files:" << e.uninstalledFiles();
        }
    }

    if (!changedEntries.isEmpty()) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed structures changed ->  rebuilding list of installed structures";
        mTool->manager()->reloadPaths();
        rebuildPluginSelectorEntries();
    }
}

QStringList StructuresManagerView::values() const
{
    return mSelectedStructures;
}

void StructuresManagerView::advancedSelection()
{
    auto* dialog = new StructuresSelectionDialog(mSelectedStructures, mTool, this);
    connect(dialog, &StructuresSelectionDialog::structuresAccepted,
            this, &StructuresManagerView::setSelectedStructures);
    dialog->open();
}

void StructuresManagerView::setSelectedStructures(const QStringList& selectedStructures)
{
    if (selectedStructures == mSelectedStructures) {
        return;
    }

    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
        << "selection changed from " << mSelectedStructures << "to" << selectedStructures;
    mSelectedStructures = selectedStructures;
    emit changed(selectedStructures);
}

void StructuresManagerView::onPluginSelectorChange(bool change)
{
    if (mRebuildingPluginsList) {
        return;
    }
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
        << "pluginselector changed: " << change;
    if (!change) {
        return;
    }
    mStructuresSelector->save();
    reloadSelectedItems();
}

void StructuresManagerView::reloadSelectedItems()
{
    QStringList newVals;
    const auto structureDefs = mTool->manager()->structureDefs();
    for (const Kasten::StructureDefinitionFile* def : structureDefs) {
        KPluginInfo info = def->pluginInfo();
        if (info.isPluginEnabled()) {
            newVals.append(QStringLiteral("\'%1\':\'*\'").arg(info.pluginName()));
        }
    }

    if (newVals != mSelectedStructures) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
            << "selection changed from " << mSelectedStructures << "to" << newVals;
        mSelectedStructures = newVals;
        emit changed(newVals);
    } else {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "no change:" << mSelectedStructures;
    }
}

void StructuresManagerView::rebuildPluginSelectorEntries()
{
    mRebuildingPluginsList = true;
    KPluginInfo::List plugins;
    KPluginInfo::List dynamicPlugins;
    const auto structureDefs = mTool->manager()->structureDefs();
    for (const Kasten::StructureDefinitionFile* def : structureDefs) {
        KPluginInfo info = def->pluginInfo();
        if (info.category() == QLatin1String("structure")) {
            plugins.append(info);
        } else if (info.category() == QLatin1String("structure/js")) {
            dynamicPlugins.append(info);
        }
    }

    // XXX is there any way to clear the plugins selector?
    auto* layoutObj = qobject_cast<QBoxLayout*>(layout());
    Q_CHECK_PTR(layoutObj);
    if (mStructuresSelector) {
        layoutObj->removeWidget(mStructuresSelector);
        delete mStructuresSelector;
    }
    mStructuresSelector = new KPluginSelector(this);
    connect(mStructuresSelector, &KPluginSelector::changed,
            this, &StructuresManagerView::onPluginSelectorChange);
    layoutObj->insertWidget(0, mStructuresSelector);

    mStructuresSelector->addPlugins(plugins, KPluginSelector::ReadConfigFile,
                                    i18n("Structure Definitions"), QStringLiteral("structure"),
                                    mTool->manager()->config());
    mStructuresSelector->addPlugins(dynamicPlugins, KPluginSelector::ReadConfigFile,
                                    i18n("Dynamic Structure Definitions"), QStringLiteral("structure/js"),
                                    mTool->manager()->config());
    mStructuresSelector->load();
    mStructuresSelector->updatePluginsState();
    mRebuildingPluginsList = false;
    reloadSelectedItems();
}
