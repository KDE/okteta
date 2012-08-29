/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef ABSTRACTSTRUCTUREPARSER_H_
#define ABSTRACTSTRUCTUREPARSER_H_

#include <QVector>
#include <QStringList>

#include "../datatypes/datainformation.h"
#include "../script/scriptlogger.h"

class TopLevelDataInformation;
namespace Kasten2
{
class StructureDefinitionFile;
}

/** For use by the parsers so that the functions don't have as many parameters */
struct ParserInfo
{
    inline ParserInfo() : logger(0), parent(0) {}
    inline ParserInfo(const ParserInfo& i) : logger(i.logger), parent(i.parent), name(i.name) {}
    inline ~ParserInfo() {}
    ScriptLogger* logger;
    DataInformation* parent;
    QString name;

    inline QString context() const { return parent ? parent->fullObjectPath() + QLatin1Char('.') + name : name; }
    inline QDebug info() const { return logger->info(context()); }
    inline QDebug warn() const { return logger->warn(context()); }
    inline QDebug error() const { return logger->error(context()); }
};

class AbstractStructureParser
{
    Q_DISABLE_COPY(AbstractStructureParser)
public:
    AbstractStructureParser(const QString& pluginName, const QString& absolutePath);
    virtual ~AbstractStructureParser();
    virtual QStringList parseStructureNames() const = 0;
    virtual QVector<TopLevelDataInformation*> parseStructures() const = 0;

    static DataInformation::DataInformationEndianess byteOrderFromString(const QString& string,
            ScriptLogger* logger);
    static QString byteOrderToString(DataInformation::DataInformationEndianess order);

protected:
    const QString mPluginName;
    const QString mAbsolutePath;
};

#endif /* ABSTRACTSTRUCTUREPARSER_H_ */
