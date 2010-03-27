/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
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

namespace Kasten
{

StructuresManager::~StructuresManager()
{
    qDeleteAll(mDefs);
}
StructuresManager::StructuresManager() :
    mDefsDir(KGlobal::dirs()->locateLocal("data", "okteta/structures/", true))
{
    mConfig = KSharedConfig::openConfig("oktetastructuresrc",
            KSharedConfig::FullConfig, "config");
    reloadPaths();
}

void StructuresManager::reloadPaths()
{
    qDeleteAll(mDefs);
    mDefs.clear();
    mLoadedFiles.clear();
    QStringList paths = KGlobal::dirs()->findAllResources("data",
            "okteta/structures/*/*.desktop", KStandardDirs::Recursive
                    | KStandardDirs::NoDuplicates);
    KPluginInfo::List plugins = KPluginInfo::fromFiles(paths, mConfig->group(
            "Plugins"));
    foreach(const KPluginInfo& info, plugins)
        {
            QFileInfo desktopPath = QFileInfo(info.entryPath());
            QFileInfo osdPath = QFileInfo(desktopPath.dir(), info.pluginName()
                    + ".osd");
            QString relPath = mDefsDir.relativeFilePath(osdPath.absoluteFilePath());
            addStructDef(relPath, info);
        }
}

void StructuresManager::addStructDef(const QString& relPath, const KPluginInfo& info)
{

    QFileInfo fileInfo(mDefsDir, relPath);
    StructureDefinitionFile* def = new StructureDefinitionFile(fileInfo, info);
    if (!mDefs.contains(relPath))
        mDefs.insert(relPath, def);
    if (info.isValid() && info.isPluginEnabled())
    {
        mLoadedFiles.append(relPath);
    }
}

}
