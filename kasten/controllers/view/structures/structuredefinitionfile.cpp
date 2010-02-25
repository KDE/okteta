/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "structuredefinitionfile.h"

#include <QFile>
#include <QDir>
#include <QDomNode>
#include <QStringList>

#include <KDebug>

#include "datatypes/structuredatainformation.h"
#include "datatypes/uniondatainformation.h"
#include "datatypes/staticlengtharraydatainformation.h"
#include "datatypes/dynamiclengtharraydatainformation.h"
#include "datatypes/primitivedatainformation.h"
#include "datatypes/enumdatainformation.h"
#include "datatypes/boolbitfielddatainformation.h"
#include "datatypes/unsignedbitfielddatainformation.h"
#include "datatypes/signedbitfielddatainformation.h"

namespace Kasten
{
StructureDefinitionFile::StructureDefinitionFile(QFileInfo file, KPluginInfo info) :
    mPluginInfo(info), mFileInfo(file), mValid(false), mLoaded(false)
{
    mDir = mFileInfo.dir();
    kDebug() << "foof";
}

StructureDefinitionFile::~StructureDefinitionFile()
{
    qDeleteAll(mTopLevelStructures);
}

StructureDefinitionFile::StructureDefinitionFile(StructureDefinitionFile& f) :
    mPluginInfo(f.mPluginInfo), mFileInfo(f.mFileInfo), mIncludedFiles(
            f.mIncludedFiles), mValid(f.mValid), mLoaded(f.mLoaded)
{
    int len = f.mTopLevelStructures.length();
    for (int i = 0; i < len; ++i)
    {
        mTopLevelStructures.append(f.mTopLevelStructures.at(i)->clone());
    }
}

void StructureDefinitionFile::parseIncludeNodes(QDomNodeList& elems)
{
    for (uint i = 0; i < elems.length(); i++)
    {
        QDomElement elem = elems.at(i).toElement();
        if (elem.isNull())
            continue;
        QString includeFile = elem.attribute("file");
        if (!includeFile.isNull())
        {
            kDebug() << "including file: " << includeFile;
            //if contains this file, file was already included
            if (!mIncludedFiles.contains(includeFile))
            {
                mIncludedFiles.append(includeFile);
            }
            else
            {
                kDebug() << "included file is already loaded";
            }
        }
    }
}

void StructureDefinitionFile::parseEnumDefNodes(QDomNodeList& elems)
{
    for (uint i = 0; i < elems.length(); i++)
    {
        QDomElement elem = elems.at(i).toElement();
        if (elem.isNull())
            continue;
        QMap<AllPrimitiveTypes, QString> defs;
        QString enumName = elem.attribute("name", i18n("<no name specified>"));
        QString typeStr = elem.attribute("type");
        if (typeStr.isNull())
        {
            kWarning() << "no type attribute defined -> skipping this enum";
            continue;
        }
        PrimitiveDataType type = PrimitiveDataInformation::typeStringToType(typeStr);
        //handle all entries
        QDomNodeList children = elem.elementsByTagName("entry");
        for (uint j = 0; j < children.length(); j++)
        {
            QDomElement child = children.at(j).toElement();
            if (child.isNull())
                continue;
            QString name = child.attribute("name", i18n("<no name specified>"));
            bool okay = false;
            QString valStr = child.attribute("value", QString());
            qlonglong val = valStr.toLongLong(&okay, 10); //TODO hex literals
            if (!okay || valStr.isEmpty())
            {
                kWarning() << "failed to parse value attribute (name=" << name
                        << ")";
                continue;
            }
            else
            {
                defs.insert(val, name);
            }
        }
        //now add this enum to the list of enums
        if (defs.size() != 0)
        {
            EnumDefinition::Ptr enumDef = EnumDefinition::Ptr(new EnumDefinition(
                    defs, enumName, type));
            mEnums.append(enumDef);
        }
    }
    kDebug() << "foo";
}
QList<DataInformation*> StructureDefinitionFile::structures() const
{
    QList<DataInformation*> list;
    foreach(const DataInformation* data,mTopLevelStructures)
        {
            list.append(data->clone());
        }
    return list;
}

void StructureDefinitionFile::parse()
{
    if (mLoaded)
    {
        kDebug() << "Already loaded -> will return";
        return;
    }
    mLoaded = true;
    QFile file(mFileInfo.absoluteFilePath());
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly))
    {
        kWarning() << "could not open file "
                << mFileInfo.dir().absoluteFilePath(mPluginInfo.name().endsWith(
                        QLatin1String(".osd")) ? mPluginInfo.name()
                        : mPluginInfo.name() + ".osd");
        return;
    }
    int errorLine, errorColumn;
    QString errorMsg;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        file.close();
        kWarning() << "DataInformation::loadFromXML(): error reading file:\n"
                << errorMsg << "\n error line=" << errorLine << " error column="
                << errorColumn;
        return;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    //    kDebug() << "root elem tag: " << docElem.tagName();
    //first find all enum definitons and includes:
    QDomNodeList enumDefs = docElem.elementsByTagName("enumDef");
    parseEnumDefNodes(enumDefs);
    QDomNodeList includeElems = docElem.elementsByTagName("include");
    parseIncludeNodes(includeElems);

    QDomNodeList list = docElem.childNodes();
    for (uint i = 0; i < list.length(); ++i)
    {
        const QDomNode& n = list.at(i);
        QDomElement elem = n.toElement(); // try to convert the node to an element.
        DataInformation* data = NULL;
        if (!elem.isNull())
        {
            kDebug() << "element tag: " << elem.tagName();
            //e is element
            QString tag = elem.tagName();
            if (tag == "enumDef" || tag == "include")
                continue;
            else
                data = parseNode(elem);
        }

        if (data)
        {
            mTopLevelStructures.append(data);
        }
        else
        {
            kDebug() << "data == NULL -> could not parse node " << elem.tagName();
        }
    }
    mLoaded = true;
    mValid = true;
    return;
}
DataInformation* StructureDefinitionFile::getStructure(QString& name) const
{
    if (!mLoaded)
        kError() << "member accessed before file was parsed";
    if (!mValid)
        kError() << "reading data from invalid file";
    foreach(const DataInformation* data,mTopLevelStructures)
        {
            if (data->name() == name)
            {
                return data->clone();
            }
        }
    return NULL; // not found
}

QStringList StructureDefinitionFile::includedFiles() const
{
    if (!mLoaded)
        kError() << "member accessed before file was parsed";
    if (!mValid)
        kError() << "reading data from invalid file";
    return mIncludedFiles;
}

//Datatypes

AbstractArrayDataInformation*
StructureDefinitionFile::arrayFromXML(const QDomElement& xmlElem) const
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    DataInformation* subElem = NULL;
    QDomNode node = xmlElem.firstChild();
    subElem = parseNode(node);
    if (!subElem)
    {
        kWarning() << "AbstractArrayDataInformation::fromXML():"
            " could not parse subelement type";
        return NULL;
    }
    QString lengthStr = xmlElem.attribute("length");
    if (lengthStr.isNull())
    {
        kWarning() << "StaticLengthPrimitiveArrayDataInformation::fromXML():"
            " no length attribute defined";
        delete subElem;
        return NULL;
    }
    AbstractArrayDataInformation* retVal;
    bool okay = true;
    int length = lengthStr.toInt(&okay, 10); //TODO dynamic length
    if (!okay)
    {
        kDebug()
                << "error parsing length string -> is dynamic length array. Length string="
                << lengthStr;
        retVal = new DynamicLengthArrayDataInformation(name, lengthStr, *subElem);
    }
    else if (length >= 0)
    {
        retVal = new StaticLengthArrayDataInformation(name, length, *subElem);
    }
    else
    {
        kWarning() << "could not parse length string:" << lengthStr;
        delete subElem;
        return NULL;
    }
    delete subElem; //control not taken over by constructor
    return retVal;
}

PrimitiveDataInformation* StructureDefinitionFile::primitiveFromXML(
        const QDomElement& xmlElem) const
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute("type", QString());
    if (typeStr.isEmpty())
    {
        kWarning()
                << "PrimitiveDataInformation::fromXML(): no type attribute defined";
        return NULL;
    }
    PrimitiveDataType type = PrimitiveDataInformation::typeStringToType(typeStr);
    return PrimitiveDataInformation::newInstance(name, type);
}

AbstractBitfieldDataInformation* StructureDefinitionFile::bitfieldFromXML(
        const QDomElement& xmlElem) const
{
    kDebug() << "loading bitfield";
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute("type", QString());
    QString widthStr = xmlElem.attribute("width", QString());
    bool okay = false;
    uint width = widthStr.toUInt(&okay,10);
    if (width == 0 || !okay)
    {
        kWarning() << "invalid width specified (width must be > 0)";
        return NULL;
    }
    if (typeStr == "bool")
        return new BoolBitfieldDataInformation(name, Type_Bitfield, width);
    if (typeStr == "unsigned")
        return new UnsignedBitfieldDataInformation(name, Type_Bitfield, width);
    if (typeStr == "signed")
        return new SignedBitfieldDataInformation(name, Type_Bitfield, width);
    else
    {
        kWarning() << "no (or invalid) type attribute defined,"
            " defaulting to unsigned";
        return new UnsignedBitfieldDataInformation(name, Type_Bitfield, width);
    }
}

UnionDataInformation* StructureDefinitionFile::unionFromXML(
        const QDomElement& xmlElem) const
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    UnionDataInformation* un = new UnionDataInformation(name);
    QDomNode node = xmlElem.firstChild();
    while (!node.isNull())
    {
        DataInformation* data = parseNode(node);
        if (data)
            un->addDataTypeToUnion(data);
        node = node.nextSibling();
    }
    return un;
}

StructureDataInformation* StructureDefinitionFile::structFromXML(
        const QDomElement& xmlElem) const
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    StructureDataInformation* stru = new StructureDataInformation(name);
    QDomNode node = xmlElem.firstChild();
    while (!node.isNull())
    {
        DataInformation* data = parseNode(node);
        if (data)
            stru->addDataTypeToStruct(data);
        node = node.nextSibling();
    }
    return stru;
}

EnumDataInformation* StructureDefinitionFile::enumFromXML(const QDomElement& xmlElem) const
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute("type", QString());
    if (typeStr.isEmpty())
    {
        kWarning() << "no type attribute defined";
        return NULL;
    }
    QString enumName = xmlElem.attribute("enum", QString());
    if (enumName.isEmpty())
    {
        kWarning() << "no enum attribute defined";
        return NULL;
    }
    EnumDefinition::Ptr def = findEnum(enumName);
    if (def.constData() == NULL)
    {
        kWarning() << "no enum with name " << enumName << "found.";
        return NULL;
    }
    kDebug() << def->name();
    PrimitiveDataType type = PrimitiveDataInformation::typeStringToType(typeStr);
    PrimitiveDataInformation* prim = PrimitiveDataInformation::newInstance(name,
            type);
    if (!prim)
    {
        kWarning() << "primitive type is null!!";
        return NULL;
    }
    EnumDataInformation* enumd = new EnumDataInformation(name, prim, def);
    if (!enumd)
        kDebug() << "enum def is NULL!!!";
    return enumd;
}

DataInformation* StructureDefinitionFile::parseNode(const QDomNode& n) const
{
    QDomElement elem = n.toElement(); // try to convert the node to an element.
    DataInformation* data = NULL;
    if (!elem.isNull())
    {
        //      kDebug() << "element tag: " << elem.tagName();
        //e is element
        if (elem.tagName() == "struct")
            data = structFromXML(elem);
        else if (elem.tagName() == "array")
            data = arrayFromXML(elem);
        else if (elem.tagName() == "bitfield")
            data = bitfieldFromXML(elem);
        else if (elem.tagName() == "primitive")
            data = primitiveFromXML(elem);
        else if (elem.tagName() == "union")
            data = unionFromXML(elem);
        if (elem.tagName() == "enum")
            data = enumFromXML(elem);
    }
    return data;
}
EnumDefinition::Ptr StructureDefinitionFile::findEnum(const QString& defName) const
{
    for (int i = 0; i < mEnums.length(); ++i)
    {
        const EnumDefinition::Ptr def = mEnums.at(i);
        if (def->name() == defName)
        {
            kDebug() << "found at index: " << i;
            return def;
        }
    }
    kDebug() << "enum " << defName << "not found in enums list";
    return EnumDefinition::Ptr(NULL);
}
}
