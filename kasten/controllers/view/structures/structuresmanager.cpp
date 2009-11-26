/*
 This file is part of the Okteta Kasten module, part of the KDE project.

 Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
}
StructuresManager::StructuresManager() :
    mDefsDir(KGlobal::dirs()->locateLocal("data", "okteta/structures/", true))
{
    mConfig = KSharedConfig::openConfig("oktetastructuresrc",
            KSharedConfig::FullConfig, "config");
    reloadPaths();
}

void StructuresManager::update()
{
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
    foreach(KPluginInfo info,plugins)
        {
            //            if (!info.isPluginEnabled())
            //                continue;
            QFileInfo desktopPath = QFileInfo(info.entryPath());
            QFileInfo osdPath = QFileInfo(desktopPath.dir(), info.pluginName()
                    + ".osd");
            QString relPath = mDefsDir.relativeFilePath(osdPath.absoluteFilePath());
            //            addStructDef(info, file);
            kDebug() << "add struct def " << relPath;
            QFileInfo fileInfo(mDefsDir, relPath);
            StructureDefinitionFile* def = new StructureDefinitionFile(fileInfo,
                    info);
            if (info.isValid() && info.isPluginEnabled())
            {
                mLoadedFiles.append(relPath);
            }
            def->parse(); //TODO lazy loading
            mDefs.insert(relPath, def);
            manageIncludes(def);
        }
    kDebug() << "loaded defs";
}

//
//void StructuresManager::manageIncludes(const StructureDefinitionFile* def)
//{
//    QStringList includedFiles = def->includedFiles();
//    if (includedFiles.length() == 0)
//        return;
//    kDebug() << "included files: " << def->includedFiles();
//    QDir defDir = QDir(def->dir().absolutePath());
//    //add included files structures
//    for (int i = 0; i < includedFiles.length(); ++i)
//    {
//        QString inclPath = includedFiles.at(i);
//        kDebug() << "include path: " << inclPath;
//        //XXX maybe more lazy loading
//        //check if included file is already loaded
//        QString relPath =
//                defsDir.relativeFilePath(defDir.absoluteFilePath(inclPath));
//        kDebug() << "rel path = " << relPath;
//        relPath = QDir::cleanPath(relPath);
//        kDebug() << "rel path = " << relPath;
//        if (mDefs.contains(relPath))
//        {
//            kDebug() << "included file already loaded: " << relPath;
//        }
//        else
//        {
//            QFileInfo test(defsDir, relPath);
//
//            if (test.exists()) // only load if file exists
//                addStructDef(KPluginInfo(), QFileInfo(relPath));
//            else
//            {
//                kDebug() << "included file does not exist: " << relPath;
//            }
//        }
//    }
//}


void StructuresManager::loadStructDefFiles()
{
    reloadPaths();
}

void StructuresManager::addStructDef(const QString& relPath)
{
    kDebug() << "add struct def " << relPath;
    QFileInfo fileInfo(mDefsDir, relPath);
    StructureDefinitionFile* def = new StructureDefinitionFile(fileInfo,
            KPluginInfo());
    def->parse(); //TODO lazy loading
    if (!mDefs.contains(relPath))
    {
        mLoadedFiles.append(relPath);
        mDefs.insert(relPath, def);
        manageIncludes(def);
    }
}

void StructuresManager::manageIncludes(const StructureDefinitionFile* def)
{
    QStringList includedFiles = def->includedFiles();
    if (includedFiles.length() == 0)
        return;
    kDebug() << "included files: " << def->includedFiles();
    QDir defDir = def->dir();
    //add included files structures
    for (int i = 0; i < includedFiles.length(); ++i)
    {
        QString inclPath = includedFiles.at(i);
        kDebug() << "include path: " << inclPath;
        //XXX maybe more lazy loading
        //check if included file is already loaded
        QString relPath = mDefsDir.relativeFilePath(
                defDir.absoluteFilePath(inclPath));
        kDebug() << "rel path = " << relPath;
        relPath = QDir::cleanPath(relPath);
        kDebug() << "rel path = " << relPath;
        if (mDefs.contains(relPath))
        {
            kDebug() << "included file already loaded: " << relPath;
        }
        else
        {
            QFileInfo test(mDefsDir, relPath);

            if (test.exists()) // only load if file exists
                addStructDef(relPath);
            else
            {
                kDebug() << "included file does not exist: " << relPath;
            }
        }
    }
}

}
