/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include "structuresmanager.h"

// tool
#include "structureviewpreferences.h"
#include "structuredefinitionfile.h"
#include "structlogging.h"
// Qt
#include <QStandardPaths>
#include <QDir>

namespace Kasten
{

StructuresManager::~StructuresManager()
{
    qDeleteAll(mDefs);
}

StructuresManager::StructuresManager(QObject* parent)
  : QObject(parent)
{
    mConfig = KSharedConfig::openConfig(QStringLiteral("oktetastructuresrc"),
            KSharedConfig::FullConfig, QStandardPaths::ConfigLocation);
    reloadPaths();
}

void StructuresManager::reloadPaths()
{
    qDeleteAll(mDefs);
    mDefs.clear();
    mLoadedFiles.clear();
    QStringList paths;
    const QStringList structuresDirs = QStandardPaths::locateAll( QStandardPaths::GenericDataLocation,
            QStringLiteral("okteta/structures"), QStandardPaths::LocateDirectory );
    foreach( const QString& structuresDir, structuresDirs )
    {
        const QStringList entries = QDir( structuresDir ).entryList( QDir::Dirs );
        foreach( const QString& e, entries )
        {
            const QString structureBasePath = structuresDir + QLatin1Char('/') + e;
            const QStringList desktopFiles =
                QDir(structureBasePath).entryList( QStringList(QStringLiteral("*.desktop")) );
            foreach(const QString& desktopFile, desktopFiles)
            {
                paths << structureBasePath + QLatin1Char('/') + desktopFile;
            }
        }
    }
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "found structures: " << paths;
    KPluginInfo::List plugins = KPluginInfo::fromFiles(paths, mConfig->group("Plugins"));
    foreach(const KPluginInfo& info, plugins)
    {
        addStructDef(info);
    }
}

void StructuresManager::addStructDef(const KPluginInfo& info)
{
    const QString pluginName = info.pluginName();
    if (mDefs.contains(pluginName))
    {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Skipping structure already loaded: " << pluginName;
        return;
    }

    StructureDefinitionFile* def = new StructureDefinitionFile(info);
    mDefs.insert(pluginName, def);
}

StructureDefinitionFile* StructuresManager::definition(const QString& pluginName) const
{
    const auto definitionIt = mDefs.find(pluginName);
    if (definitionIt != mDefs.end())
    {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not find structuredefinitionFile with name=" << pluginName;
        return nullptr;
    }

    return definitionIt.value();
}

}
