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

class OsdParser: public AbstractStructureParser
{
    Q_DISABLE_COPY(OsdParser)
public:
    //TODO remove this from both osdparser and abstractstructureparser
    OsdParser(const Kasten2::StructureDefinitionFile* const def);
    /** construct a parser which opens @p file in @p dir */
    OsdParser(const QString dir, const QString file);
    /** construct a parser from an in-memory string */
    OsdParser(const QString xml);
    virtual ~OsdParser();

    virtual QStringList parseStructureNames();
    virtual QVector<TopLevelDataInformation*> parseStructures();
private:
    PrimitiveDataInformation* primitiveFromXML(const QDomElement& xmlElem) const;
    AbstractBitfieldDataInformation* bitfieldFromXML(const QDomElement& xmlElem) const;
    AbstractEnumDataInformation* enumFromXML(const QDomElement& xmlElem, bool isFlags) const;
    StringDataInformation* stringFromXML(const QDomElement& xmlElem) const;
    UnionDataInformation* unionFromXML(const QDomElement& xmlElem, QScriptEngine* engine) const;
    StructureDataInformation* structFromXML(const QDomElement& xmlElem, QScriptEngine* engine) const;
    ArrayDataInformation* arrayFromXML(const QDomElement& xmlElem, const DataInformation* parent, QScriptEngine* engine) const;

    DataInformation* parseNode(const QDomNode& node, const DataInformation* parent, QScriptEngine* engine) const;
    EnumDefinition::Ptr findEnum(const QString& defName) const;

    //void parseIncludeNodes(QDomNodeList& elems);
    void parseEnumDefNodes(QDomNodeList& elems) const;
    void parseEnums() const;
    void openDocFromFile();
private:
    QDomDocument mDocument;
    QDir mDir;
    QString mFile;
    //mutable, since they are only used for caching
    mutable QVector<EnumDefinition::Ptr> mEnums;
    mutable bool mEnumsParsed :1;
};

#endif /* OSDPARSER_H_ */
