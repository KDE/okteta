/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "structviewpreferences.h"
// tool
#include "structuredefinitionfile.h"
#include <KStandardDirs>
#include <KDebug>
#include <QDir>

namespace Kasten2
{

StructuresManager::~StructuresManager()
{
    qDeleteAll(mDefs);
}
StructuresManager::StructuresManager() :
    mDefsDir(KGlobal::dirs()->locateLocal("data", QLatin1String("okteta/structures/"), true))
{
    mConfig = KSharedConfig::openConfig(QLatin1String("oktetastructuresrc"),
            KSharedConfig::FullConfig, "config");
    reloadPaths();
}

void StructuresManager::reloadPaths()
{
    qDeleteAll(mDefs);
    mDefs.clear();
    mLoadedFiles.clear();
    QStringList paths = KGlobal::dirs()->findAllResources("data",
            QLatin1String("okteta/structures/*/*.desktop"), KStandardDirs::Recursive
                    | KStandardDirs::NoDuplicates);
    kDebug() << "found structures: " << paths;
    KPluginInfo::List plugins = KPluginInfo::fromFiles(paths, mConfig->group(
            "Plugins"));
    foreach(const KPluginInfo& info, plugins)
        {
            QFileInfo desktopPath = QFileInfo(info.entryPath());
            addStructDef(info);
        }
}

void StructuresManager::addStructDef(const KPluginInfo& info)
{

    StructureDefinitionFile* def = new StructureDefinitionFile(info);
    QString pluginName = info.pluginName();
    if (!mDefs.contains(pluginName))
        mDefs.insert(pluginName, def);
    else
        delete def;
}

StructureDefinitionFile* StructuresManager::definition(QString& pluginName)
{
    if (!mDefs.contains(pluginName))
    {
        kWarning() << "could not find structuredefinitionFile with name="
                << pluginName;
        return NULL;
    }
    return mDefs.value(pluginName);
}

}
