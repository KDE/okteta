/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESMANAGER_HPP
#define KASTEN_STRUCTURESMANAGER_HPP

#include "structuredefinitionfile.hpp"
// Qt
#include <QDir>
// KF
#include <KSharedConfig>
#include <KPluginInfo>

namespace Kasten {

class StructuresManager
{
public:
    StructuresManager();
    ~StructuresManager();

public:
    void reloadPaths();

public:
    QMap<QString, StructureDefinitionFile*> structureDefs() const;

    KSharedConfig::Ptr config() const;

    StructureDefinitionFile* definition(const QString& pluginName) const;

private:
    void addStructDef(const KPluginInfo& info);

private:
    QMap<QString, StructureDefinitionFile*> mDefs;
    KSharedConfig::Ptr mConfig;
};

}

#endif
