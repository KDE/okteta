/*
    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuresmanager.hpp"

// tool
#include "structureinstalljob.hpp"
#include "structureuninstalljob.hpp"
#include "structureviewpreferences.hpp"
#include "structuredefinitionfile.hpp"
#include <structureslogging.hpp>
// Qt
#include <QStandardPaths>
#include <QDir>
// Std
#include <algorithm>
#include <array>

namespace Kasten {

StructuresManager::StructuresManager()
{
    mConfig = KSharedConfig::openConfig(QStringLiteral("oktetastructuresrc"),
                                        KSharedConfig::FullConfig, QStandardPaths::ConfigLocation);
    reloadPaths();
}

StructuresManager::~StructuresManager() = default;

QStringList StructuresManager::installableMimeTypes()
{
    return QStringList{
        QStringLiteral("application/x-tar"),
        QStringLiteral("application/x-compressed-tar"),
        QStringLiteral("application/x-bzip2-compressed-tar"),
        QStringLiteral("application/x-tarz"),
        QStringLiteral("application/zip"),
        QStringLiteral("inode/directory"),
    };
}

QString StructuresManager::userStructuresRootDir()
{
    static const QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/okteta/structures/");
    return dir;
}

StructureInstallJob* StructuresManager::installStructureFromFile(const QUrl& structureFileUrl)
{
    auto* const job = new StructureInstallJob(this, structureFileUrl);

    return job;
}

StructureUninstallJob* StructuresManager::uninstallStructure(const QString& structureId)
{
    auto* const job = new StructureUninstallJob(this, structureId);

    return job;
}

void StructuresManager::reloadPaths()
{
    mDefs.clear();
    QStringList paths;
    QStringList backwardCompatPaths;
    const QStringList structuresDirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                                 QStringLiteral("okteta/structures"), QStandardPaths::LocateDirectory);
    for (const QString& structuresDir : structuresDirs) {
        const QStringList entries = QDir(structuresDir).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& e : entries) {
            const QString structureBasePath = structuresDir + QLatin1Char('/') + e;
            QDir structureDir(structureBasePath);
            const std::array<QString, 2> desktopFileNames {
                QStringLiteral("metadata.desktop"), // prefer metadata.desktop
                e + QLatin1String(".desktop"),
            };
            for (const QString& desktopFileName : desktopFileNames) {
                if (structureDir.exists(desktopFileName)) {
                    paths << structureBasePath + QLatin1Char('/') + desktopFileName;
                    break;
                }
            }
        }
        // Support backward-compat also files in the top-level dir, when the use the same base name
        {
            const QString structureBasePath = structuresDir;
            const QStringList desktopFiles =
                QDir(structureBasePath).entryList(QStringList(QStringLiteral("*.desktop")));
            for (const QString& desktopFile : desktopFiles) {
                backwardCompatPaths << structureBasePath + QLatin1Char('/') + desktopFile;
            }
        }
    }

    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "found structures: " << paths << backwardCompatPaths;

    for (const QString& path : std::as_const(paths)) {
        StructureMetaData structure(path);
        addStructDef(structure, StructureDefinitionFile::Location::SubDir);
    }
    for (const QString& path : std::as_const(backwardCompatPaths)) {
        StructureMetaData structure(path);
        addStructDef(structure, StructureDefinitionFile::Location::ToplevelDir);
    }
}

void StructuresManager::addStructDef(const StructureMetaData& metaData, StructureDefinitionFile::Location location)
{
    const QString id = metaData.id();
    const auto it = mDefs.find(id);
    if (it != mDefs.end()) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Skipping structure already loaded: " << id;
        return;
    }

    mDefs.emplace(id, std::make_unique<StructureDefinitionFile>(metaData, location));
}

StructureDefinitionFile* StructuresManager::definition(const QString& pluginName) const
{
    const auto definitionIt = mDefs.find(pluginName);
    if (definitionIt == mDefs.end()) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not find structuredefinitionFile with name=" << pluginName;
        return nullptr;
    }

    return definitionIt->second.get();
}

const std::map<QString, std::unique_ptr<StructureDefinitionFile>>& StructuresManager::structureDefs() const
{
    return mDefs;
}

KSharedConfig::Ptr StructuresManager::config() const
{
    return mConfig;
}

}
