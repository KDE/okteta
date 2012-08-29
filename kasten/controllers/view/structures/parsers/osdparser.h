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

#ifndef OSDPARSER_H_
#define OSDPARSER_H_

#include <QtCore/QDir>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

#include "../datatypes/primitive/enumdefinition.h"
#include "abstractstructureparser.h"

class StringDataInformation;
class ArrayDataInformation;
class AbstractBitfieldDataInformation;
class AbstractEnumDataInformation;
class DataInformation;
class StructureDataInformation;
class UnionDataInformation;
class PrimitiveDataInformation;
class ScriptLogger;

class OsdParser : public AbstractStructureParser
{
    Q_DISABLE_COPY(OsdParser)
public:
    /** construct a parser which opens parses @p absolutePath */
    OsdParser(const QString& pluginName, const QString& absolutePath);
    /** construct a parser from an in-memory string */
    OsdParser(const QString& xml);
    virtual ~OsdParser();

    virtual QStringList parseStructureNames() const;
    virtual QVector<TopLevelDataInformation*> parseStructures() const;

private:
    struct OsdParserInfo : public ParserInfo {
        inline OsdParserInfo(const QString& name, ScriptLogger* logger, DataInformation* parent,
                QScriptEngine* engine, QVector<EnumDefinition::Ptr> enums)
            : ParserInfo(name, logger, parent), engine(engine), enums(enums), scriptEngineNeeded(false) {}
        inline OsdParserInfo(const OsdParserInfo& i)
            : ParserInfo(i), engine(i.engine), enums(i.enums), scriptEngineNeeded(i.scriptEngineNeeded) {}
        inline ~OsdParserInfo() {}
        /** OSD parser only since QScriptValue::engine() exists */
        QScriptEngine* engine;
        QVector<EnumDefinition::Ptr> enums;
        bool scriptEngineNeeded;
    };

    PrimitiveDataInformation* primitiveFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const;
    AbstractBitfieldDataInformation* bitfieldFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const;
    AbstractEnumDataInformation* enumFromXML(const QDomElement& xmlElem, bool isFlags,
            const OsdParserInfo& info) const;
    StringDataInformation* stringFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const;

    template<class T> T* structOrUnionFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const;
    UnionDataInformation* unionFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const;
    StructureDataInformation* structFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const;

    ArrayDataInformation* arrayFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const;

    DataInformation* parseNode(const QDomNode& node, OsdParserInfo& info) const;

    EnumDefinition::Ptr findEnum(const QString& defName, const OsdParserInfo& info) const;

    QVector<EnumDefinition::Ptr> parseEnums(const QDomElement& rootElem,
            ScriptLogger* logger) const;

    QDomDocument openDoc(ScriptLogger* logger) const;
    QDomDocument openDocFromFile(ScriptLogger* logger) const;
    QDomDocument openDocFromString(ScriptLogger* logger) const;

private:
    /** @return the element as an XML string (excluding children) */
    QString toRawXML(const QDomElement& elem) const;

    /** if not empty construct the document from this, instead of opening file */
    const QString mXmlString;
};

inline QDomDocument OsdParser::openDoc(ScriptLogger* logger) const
{
    return mXmlString.isEmpty() ? openDocFromFile(logger) : openDocFromString(logger);
}

inline UnionDataInformation* OsdParser::unionFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const
{
    return structOrUnionFromXML<UnionDataInformation>(xmlElem, info);
}

inline StructureDataInformation* OsdParser::structFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const
{
    return structOrUnionFromXML<StructureDataInformation>(xmlElem, info);
}


#endif /* OSDPARSER_H_ */
