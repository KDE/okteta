/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSTRUCTUREPARSER_HPP
#define KASTEN_ABSTRACTSTRUCTUREPARSER_HPP

// Qt
#include <QStringList>
// Std
#include <memory>
#include <vector>

class TopLevelDataInformation;

namespace Kasten {
class StructureDefinitionFile;
}

class AbstractStructureParser
{
public:
    AbstractStructureParser(const QString& pluginName, const QString& absolutePath);
    AbstractStructureParser(const AbstractStructureParser&) = delete;
    AbstractStructureParser(AbstractStructureParser&&) = delete;

    virtual ~AbstractStructureParser();

    AbstractStructureParser& operator=(const AbstractStructureParser&) = delete;
    AbstractStructureParser& operator=(AbstractStructureParser&&) = delete;

public: // API to implement
    [[nodiscard]]
    virtual QStringList parseStructureNames() const = 0;
    [[nodiscard]]
    virtual std::vector<std::unique_ptr<TopLevelDataInformation>> parseStructures() const = 0;

protected:
    const QString mPluginName;
    const QString mAbsolutePath;
};

#endif /* KASTEN_ABSTRACTSTRUCTUREPARSER_HPP */
