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

#ifndef STRUCTURESMANAGER_H
#define STRUCTURESMANAGER_H

#include "structuredefinitionfile.h"
// Qt
#include <QList>
#include <QStringList>
#include <QDir>
// KF5
#include <KSharedConfig>
#include <KPluginInfo>

namespace Kasten
{

class StructuresManager: public QObject
{
Q_OBJECT
public:
    explicit StructuresManager(QObject* parent = nullptr);
    ~StructuresManager() override;

public Q_SLOTS:
    void reloadPaths();
public:
    inline const QStringList paths() const
    {
        return mDefs.keys();
    }

    inline const QList<StructureDefinitionFile*> structureDefs() const
    {
        return mDefs.values();
    }

    inline KSharedConfig::Ptr config() const
    {
        return mConfig;
    }
    StructureDefinitionFile* definition(QString& pluginName);
private:
    void addStructDef(const KPluginInfo& info);
private:
    QMap<QString, StructureDefinitionFile*> mDefs;
    QStringList mLoadedFiles;
    KSharedConfig::Ptr mConfig;
};

}

#endif
