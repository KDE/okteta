/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuresmanager.hpp"

// tool
#include "structureviewpreferences.hpp"
#include "structuredefinitionfile.hpp"
#include <structureslogging.hpp>
// Qt
#include <QStandardPaths>
#include <QDir>
// Std
#include <algorithm>

namespace Kasten {

StructuresManager::StructuresManager(QObject* parent)
    : QObject(parent)
{
    mConfig = KSharedConfig::openConfig(QStringLiteral("oktetastructuresrc"),
                                        KSharedConfig::FullConfig, QStandardPaths::ConfigLocation);
    reloadPaths();
}

StructuresManager::~StructuresManager()
{
    qDeleteAll(mDefs);
}

void StructuresManager::reloadPaths()
{
    qDeleteAll(mDefs);
    mDefs.clear();
    mLoadedFiles.clear();
    QStringList paths;
    const QStringList structuresDirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                                 QStringLiteral("okteta/structures"), QStandardPaths::LocateDirectory);
    for (const QString& structuresDir : structuresDirs) {
        const QStringList entries = QDir(structuresDir).entryList(QDir::Dirs);
        for (const QString& e : entries) {
            const QString structureBasePath = structuresDir + QLatin1Char('/') + e;
            const QStringList desktopFiles =
                QDir(structureBasePath).entryList(QStringList(QStringLiteral("*.desktop")));
            for (const QString& desktopFile : desktopFiles) {
                paths << structureBasePath + QLatin1Char('/') + desktopFile;
            }
        }
    }

    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "found structures: " << paths;

    for (const QString& path : std::as_const(paths)) {
        StructureMetaData structure(path);
        addStructDef(structure);
    }
}

void StructuresManager::addStructDef(const StructureMetaData& metaData)
{
    const QString id = metaData.id();
    if (mDefs.contains(id)) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Skipping structure already loaded: " << id;
        return;
    }

    auto* def = new StructureDefinitionFile(metaData);
    mDefs.insert(id, def);
}

StructureDefinitionFile* StructuresManager::definition(const QString& pluginName) const
{
    const auto definitionIt = mDefs.find(pluginName);
    if (definitionIt == mDefs.end()) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not find structuredefinitionFile with name=" << pluginName;
        return nullptr;
    }

    return definitionIt.value();
}

QMap<QString, StructureDefinitionFile*> StructuresManager::structureDefs() const
{
    return mDefs;
}

KSharedConfig::Ptr StructuresManager::config() const
{
    return mConfig;
}

}
