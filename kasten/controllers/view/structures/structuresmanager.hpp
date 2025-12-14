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
// Std
#include <map>
#include <memory>

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
    const std::map<QString, std::unique_ptr<StructureDefinitionFile>>& structureDefs() const;

    [[nodiscard]]
    KSharedConfig::Ptr config() const;

    [[nodiscard]]
    StructureDefinitionFile* definition(const QString& pluginName) const;

private:
    void addStructDef(const StructureMetaData& metaData);

private:
    std::map<QString, std::unique_ptr<StructureDefinitionFile>> mDefs;
    KSharedConfig::Ptr mConfig;
};

}

#endif
