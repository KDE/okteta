/*
    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESMANAGER_HPP
#define KASTEN_STRUCTURESMANAGER_HPP

// tool
#include "structuredefinitionfile.hpp"
// KF
#include <KSharedConfig>
// Qt
#include <QStringList>
#include <QDir>

class StructureMetaData;

namespace Kasten {

class StructuresManager
{
public:
    StructuresManager();
    ~StructuresManager();

public:
    void reloadPaths();

public:
    [[nodiscard]]
    QMap<QString, StructureDefinitionFile*> structureDefs() const;

    [[nodiscard]]
    KSharedConfig::Ptr config() const;

    [[nodiscard]]
    StructureDefinitionFile* definition(const QString& pluginName) const;

private:
    void addStructDef(const StructureMetaData& metaData);

private:
    QMap<QString, StructureDefinitionFile*> mDefs;
    QStringList mLoadedFiles;
    KSharedConfig::Ptr mConfig;
};

}

#endif
