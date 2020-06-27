/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSTRUCTUREPARSER_HPP
#define KASTEN_ABSTRACTSTRUCTUREPARSER_HPP

#include <QVector>
#include <QStringList>

class TopLevelDataInformation;

namespace Kasten {
class StructureDefinitionFile;
}

class AbstractStructureParser
{
    Q_DISABLE_COPY(AbstractStructureParser)

public:
    AbstractStructureParser(const QString& pluginName, const QString& absolutePath);
    virtual ~AbstractStructureParser();
    virtual QStringList parseStructureNames() const = 0;
    virtual QVector<TopLevelDataInformation*> parseStructures() const = 0;

protected:
    const QString mPluginName;
    const QString mAbsolutePath;
};

#endif /* KASTEN_ABSTRACTSTRUCTUREPARSER_HPP */
