/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

#include "../datatypes/enumdefinition.h"
#include "abstractstructureparser.h"

class AbstractArrayDataInformation;
class AbstractBitfieldDataInformation;
class EnumDataInformation;
class DataInformation;
class StructureDataInformation;
class UnionDataInformation;
class PrimitiveDataInformation;

class OsdParser: public AbstractStructureParser
{
public:
    OsdParser(const Kasten::StructureDefinitionFile* const def);
    virtual ~OsdParser();

    virtual QStringList parseStructureNames();
    virtual QList<const TopLevelDataInformation*> parseStructures();
    virtual bool isFullyParsed();

private:
    void parseEnums();

    AbstractArrayDataInformation* arrayFromXML(const QDomElement& node);
    PrimitiveDataInformation* primitiveFromXML(const QDomElement& node);
    AbstractBitfieldDataInformation
    * bitfieldFromXML(const QDomElement& xmlElem);
    UnionDataInformation* unionFromXML(const QDomElement& node);
    StructureDataInformation* structFromXML(const QDomElement& node);
    EnumDataInformation* enumFromXML(const QDomElement& node);

    DataInformation* parseNode(const QDomNode& node);
    EnumDefinition::Ptr findEnum(const QString& defName);

    //    void parseIncludeNodes(QDomNodeList& elems);
    void parseEnumDefNodes(QDomNodeList& elems);
    QDomDocument openFile();
private:
    QList<EnumDefinition::Ptr> mEnums;
    bool mEnumsParsed :1;
    bool mFullyParsed :1;
};

#endif /* OSDPARSER_H_ */
