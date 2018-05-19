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

#include "abstractstructureparser.h"
#include "parserutils.h"
#include "../datatypes/primitive/enumdefinition.h"

#include <QDomElement>

class QDomDocument;

class StringDataInformation;
class ArrayDataInformation;
class AbstractBitfieldDataInformation;
class EnumDataInformation;
class DataInformation;
class StructureDataInformation;
class UnionDataInformation;
class PrimitiveDataInformation;
class ScriptLogger;

struct OsdParserInfo : public ParserInfo
{
    inline OsdParserInfo(const QString& name, ScriptLogger* logger, DataInformation* parent,
                         QScriptEngine* engine, const QVector<EnumDefinition::Ptr>& enums)
        : ParserInfo(name, logger, parent, engine)
        , enums(enums)
    {}
    inline OsdParserInfo(const OsdParserInfo& i)
        : ParserInfo(i)
        , enums(i.enums)
    {}
    inline ~OsdParserInfo() {}
    QVector<EnumDefinition::Ptr> enums;
};

class OsdParser : public AbstractStructureParser
{
    Q_DISABLE_COPY(OsdParser)

public:
    /** construct a parser which opens parses @p absolutePath */
    OsdParser(const QString& pluginName, const QString& absolutePath);
    /** construct a parser from an in-memory string */
    explicit OsdParser(const QString& xml);
    ~OsdParser() override;

    QStringList parseStructureNames() const override;
    QVector<TopLevelDataInformation*> parseStructures() const override;

    static DataInformation* parseElement(const QDomElement& node, const OsdParserInfo& info);

private:
    static PrimitiveDataInformation* primitiveFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static PointerDataInformation* pointerFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static AbstractBitfieldDataInformation* bitfieldFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static EnumDataInformation* enumFromXML(const QDomElement& elem, bool isFlags, const OsdParserInfo& info);
    static StringDataInformation* stringFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static UnionDataInformation* unionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static StructureDataInformation* structFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static ArrayDataInformation* arrayFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    static TaggedUnionDataInformation* taggedUnionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    /** Get the child type from the <type> element or type="" attribute.
     * This handles both the case where it is passed as an XML element and as a primitive type string
     * @param xmlElem the parent XML element
     * @param info the parser info
     * @return The parsed element or null if not possible. */
    static DataInformation* parseType(const QDomElement& xmlElem, const OsdParserInfo& info, const QString& name);
    static DataInformation* parseChildElement(const QDomElement& xmlElem, const OsdParserInfo& info, const QString& name);

    static EnumDefinition::Ptr findEnum(const QString& defName, const OsdParserInfo& info);

    static QVector<EnumDefinition::Ptr> parseEnums(const QDomElement& rootElem, ScriptLogger* logger);

    QDomDocument openDoc(ScriptLogger* logger) const;
    QDomDocument openDocFromFile(ScriptLogger* logger) const;
    QDomDocument openDocFromString(ScriptLogger* logger) const;

    /** Reads an property of the QDomElement. First it is checked whether an attribute exists, if this is not the case
     * the inner text of an element with tag equal to @p attrib is returned*/
    static QString readProperty(const QDomElement& elem, const QString& attrib, const QString& defaultVal = QString());
    /** if not empty construct the document from this, instead of opening file */
    const QString mXmlString;
};

class OsdChildrenParser : public ChildrenParser
{
public:
    OsdChildrenParser(const OsdParserInfo& info, const QDomElement& firstChild);
    ~OsdChildrenParser() override;
    DataInformation* next() override;
    bool hasNext() override;
    void setParent(DataInformation* newParent) override;

protected:
    OsdParserInfo mInfo;
    QDomElement mElem;
};

class SingleElementOsdChildrenParser : public OsdChildrenParser
{
public:
    SingleElementOsdChildrenParser(const OsdParserInfo& info, const QDomElement& element);
    ~SingleElementOsdChildrenParser() override;
    DataInformation* next() override;
    bool hasNext() override;

protected:
    bool mParsed;
};

#endif /* OSDPARSER_H_ */
