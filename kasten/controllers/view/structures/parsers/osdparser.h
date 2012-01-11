/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

class OsdParser: public AbstractStructureParser
{
public:
    //TODO remove this from both osdparser and abstractstructureparser
    OsdParser(const Kasten2::StructureDefinitionFile* const def);
    /** construct a parser which opens @p file in @p dir */
    OsdParser(const QString dir, const QString file);
    /** construct a parser from an in-memory string */
    OsdParser(const QString xml);
    virtual ~OsdParser();

    virtual QStringList parseStructureNames();
    virtual QList<const TopLevelDataInformation*> parseStructures();
    virtual bool isFullyParsed() const;

private:
    void parseEnums();
    PrimitiveDataInformation* primitiveFromXML(const QDomElement& node);
    AbstractBitfieldDataInformation* bitfieldFromXML(const QDomElement& node);
    AbstractEnumDataInformation* enumFromXML(const QDomElement& xmlElem, bool isFlags);
    StringDataInformation* stringFromXML(const QDomElement& node);
    UnionDataInformation* unionFromXML(const QDomElement& xmlElem);
    StructureDataInformation* structFromXML(const QDomElement& xmlElem);
    ArrayDataInformation* arrayFromXML(const QDomElement& node, const DataInformation* parent);

    DataInformation* parseNode(const QDomNode& node, const DataInformation* parent);
    EnumDefinition::Ptr findEnum(const QString& defName);

    //void parseIncludeNodes(QDomNodeList& elems);
    void parseEnumDefNodes(QDomNodeList& elems);
    void openDocFromFile();
private:
    QDomDocument mDocument;
    QDir mDir;
    QString mFile;
    QScriptEngine* mEngine; //maybe needed for dynamic arrays
    QList<EnumDefinition::Ptr> mEnums;
    bool mEnumsParsed :1;
    bool mFullyParsed :1;
};

#endif /* OSDPARSER_H_ */
