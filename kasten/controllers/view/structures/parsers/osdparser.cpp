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

#include "osdparser.h"

#include "../datatypes/staticlengtharraydatainformation.h"
#include "../datatypes/dynamiclengtharraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/enumdatainformation.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../datatypes/strings/stringdata.h"
#include "../datatypes/primitive/bitfield/boolbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "../datatypes/topleveldatainformation.h"

#include "../datatypes/primitivefactory.h"

#include "../structuredefinitionfile.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

OsdParser::OsdParser(const Kasten::StructureDefinitionFile* const def) :
    AbstractStructureParser(def), mEnumsParsed(false), mFullyParsed(false)
{
}

OsdParser::~OsdParser()
{
}

QStringList OsdParser::parseStructureNames()
{
    QStringList ret;
    QDomDocument doc = openFile();
    QDomNode rootElem = doc.firstChildElement("data");
    QDomNodeList nodes = rootElem.childNodes();
    for (uint i = 0; i < nodes.length(); ++i)
    {
        QDomElement elem = nodes.at(i).toElement(); // try to convert the node to an element.
        if (!elem.isNull())
        {
            //      kDebug() << "element tag: " << elem.tagName();
            //e is element
            QString tag = elem.tagName();
            if (tag == "struct" || tag == "array" || tag == "bitfield" || tag
                    == "primitive" || tag == "union" || tag == "enum")
                ret.append(elem.attribute("name", "<invalid name>"));
        }
    }
    return ret;
}

QDomDocument OsdParser::openFile()
{
    QFileInfo fileInfo(mDef->dir(), mDef->pluginInfo().pluginName() + QLatin1String(
            ".osd"));
    if (!fileInfo.exists())
    {
        kWarning() << "Structure definition " << fileInfo.absoluteFilePath()
                << " does not exist";
    }
    QFile file(fileInfo.absoluteFilePath());
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly))
    {
        kWarning() << "could not open file " << fileInfo.absoluteFilePath();
        return QDomDocument();
    }
    int errorLine, errorColumn;
    QString errorMsg;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        file.close();
        kWarning()
                << "TopLevelDataInformation::loadFromXML(): error reading file:\n"
                << errorMsg << "\n error line=" << errorLine << " error column="
                << errorColumn;
        return QDomDocument();
    }
    file.close();
    return doc;

}
QList<const TopLevelDataInformation*> OsdParser::parseStructures()
{
    QFileInfo fileInfo(mDef->dir(), mDef->pluginInfo().pluginName() + QLatin1String(
            ".osd"));
    if (!fileInfo.exists())
    {
        kWarning() << "file " << fileInfo.absoluteFilePath() << " doesn't exist";
        return QList<const TopLevelDataInformation*> ();
    }
    QDomDocument doc = openFile();
    QDomElement rootElem = doc.firstChildElement("data");
    if (!mEnumsParsed)
    {
        QDomNodeList enumDefs = rootElem.elementsByTagName("enumDef");
        parseEnumDefNodes(enumDefs);
        mEnumsParsed = true;
    }
    QList<const TopLevelDataInformation*> structures;
    QDomNodeList list = rootElem.childNodes();
    for (uint i = 0; i < list.length(); ++i)
    {
        const QDomNode& n = list.at(i);
        QDomElement elem = n.toElement(); // try to convert the node to an element.
        if (elem.isNull())
            continue;

        //e is element
        kDebug()
            << "element tag: " << elem.tagName();
        QString tag = elem.tagName();
        DataInformation* data = parseNode(elem);
        if (data)
        {
            TopLevelDataInformation* topData = new TopLevelDataInformation(data,
                    fileInfo);
            structures.append(topData);
        }
        else
        {
            kDebug()
                << "data == NULL -> could not parse node " << elem.tagName();
            delete data;
        }
    }
    mFullyParsed = true; //however if this method is called again the result will not be cached
    return structures;
}

bool OsdParser::isFullyParsed()
{
    return mFullyParsed;
}

void OsdParser::parseEnums()
{
    if (mEnumsParsed)
        return;
    QDomDocument doc = openFile();
    QDomNodeList enumDefs = doc.firstChildElement("data").elementsByTagName(
            "enumDef");
    parseEnumDefNodes(enumDefs);
    mEnumsParsed = true;
}

//Datatypes

AbstractArrayDataInformation*
OsdParser::arrayFromXML(const QDomElement& xmlElem)
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    QDomNode node = xmlElem.firstChild();
    QScopedPointer<DataInformation> subElem(parseNode(node));
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
        return NULL;
    }
    AbstractArrayDataInformation* retVal;
    bool okay = true;
    int length = lengthStr.toInt(&okay, 10); //TODO dynamic length
    if (!okay)
    {
        kDebug() << "error parsing length string -> is dynamic length array. Length string="
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
        return NULL;
    }
    return retVal;
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem)
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute("type", QString());
    if (typeStr.isEmpty())
    {
        kWarning()
                << "PrimitiveDataInformation::fromXML(): no type attribute defined";
        return NULL;
    }
    return PrimitiveFactory::newInstance(name, typeStr);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(
        const QDomElement& xmlElem)
{
    kDebug()
        << "loading bitfield";
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute("type", QString());
    QString widthStr = xmlElem.attribute("width", QString());
    bool okay = false;
    uint width = widthStr.toUInt(&okay, 10);
    if (width == 0 || !okay)
    {
        kWarning() << "invalid width specified (width must be > 0)";
        return NULL;
    }
    AbstractBitfieldDataInformation* bitf = 0;
    if (typeStr == "bool")
        bitf = new BoolBitfieldDataInformation(name, width);
    else if (typeStr == "unsigned")
        bitf = new UnsignedBitfieldDataInformation(name, width);
    else if (typeStr == "signed")
        bitf = new SignedBitfieldDataInformation(name, width);
    else
    {
        kWarning() << "no (or invalid) bitfield type attribute defined:" << typeStr
             << " defaulting to unsigned";
        bitf = new UnsignedBitfieldDataInformation(name, width);
    }
    return bitf;
}

UnionDataInformation* OsdParser::unionFromXML(const QDomElement& xmlElem)
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

StructureDataInformation* OsdParser::structFromXML(const QDomElement& xmlElem)
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

EnumDataInformation* OsdParser::enumFromXML(const QDomElement& xmlElem)
{
    if (!mEnumsParsed) //not always needed
        parseEnums();

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
    PrimitiveDataInformation* prim = PrimitiveFactory::newInstance(name, typeStr);
    if (!prim)
    {
        kWarning() << "primitive type is null!!";
        return NULL;
    }
    EnumDataInformation* enumd = new EnumDataInformation(name, prim, def);
    if (!enumd)
        kError() << "enum def is NULL!!!";
    return enumd;
}

StringDataInformation* OsdParser::stringFromXML(const QDomElement& node)
{
    const QString name = node.attribute("name", i18n("<invalid name>"));
    const QString terminatedBy = node.attribute("terminatedBy");
    const QString charCount = node.attribute("charCount");
    const QString byteCount = node.attribute("byteCount");
    const QString encoding = node.attribute("encoding");

    StringData::TerminationModes mode = StringData::None;
    if (!terminatedBy.isEmpty())
        mode |= StringData::Sequence;
    if (!charCount.isEmpty())
        mode |= StringData::CharCount;
    if (!byteCount.isEmpty())
        mode |= StringData::ByteCount;

    if ((mode & StringData::CharCount) && (mode & StringData::ByteCount))
        mode &= ~StringData::ByteCount; //when both exists charcount wins

    StringDataInformation* data = new StringDataInformation(name, encoding);
    //if mode is None, we assume zero terminated strings
    bool ok;
    if (mode == StringData::None)
    {
        mode = StringData::Sequence;
        data->setTerminationCodePoint(0);
    }
    else if (mode & StringData::Sequence)
    {
        uint term;
        if (terminatedBy.startsWith(QLatin1String("0x")))
            term = terminatedBy.mid(2).toUInt(&ok, 16);
        else
            term = terminatedBy.toUInt(&ok, 16); // always a hex value, never okay as normal val

        if (!ok)
            kDebug() << "invalid termination codepoint specified " << terminatedBy
                << " (should be a hex number). Defaulting to 0";
        data->setTerminationCodePoint(term);
    }
    if (mode & StringData::CharCount)
    {
        uint count;
        if (charCount.startsWith(QLatin1String("0x")))
            count = charCount.mid(2).toUInt(&ok, 16);
        else
            count = charCount.toUInt(&ok, 10);

        if (!ok)
        {
            kDebug() << "unparseable char count: " << charCount << ", defaulting to 1";
            count = 1;
        }
        data->setMaxCharCount(count);
    }
    if (mode & StringData::ByteCount)
    {
        uint count;
        if (byteCount.startsWith(QLatin1String("0x")))
            count = byteCount.mid(2).toUInt(&ok, 16);
        else
            count = byteCount.toUInt(&ok, 10);

        if (!ok)
        {
            kDebug() << "unparseable char count: " << byteCount << ", defaulting to 1";
            count = 1;
        }
        data->setMaxByteCount(count);
    }
    return data;
}

DataInformation* OsdParser::parseNode(const QDomNode& n)
{
    QDomElement elem = n.toElement(); // try to convert the node to an element.
    DataInformation* data = NULL;
    if (!elem.isNull())
    {
        //      kDebug() << "element tag: " << elem.tagName();
        //e is element
        const QString tag = elem.tagName();
        if (tag == "struct")
            data = structFromXML(elem);
        else if (tag == "array")
            data = arrayFromXML(elem);
        else if (tag == "bitfield")
            data = bitfieldFromXML(elem);
        else if (tag == "primitive")
            data = primitiveFromXML(elem);
        else if (tag == "union")
            data = unionFromXML(elem);
        else if (tag == "enum")
            data = enumFromXML(elem);
        else if (tag == "string")
            data = stringFromXML(elem);
    }
    if (data) {
        QString byteOrder = elem.attribute("byteOrder", "inherit");
        data->setByteOrder(byteOrderFromString(byteOrder));
    }
    return data;
}

EnumDefinition::Ptr OsdParser::findEnum(const QString& defName)
{
    for (int i = 0; i < mEnums.length(); ++i)
    {
        const EnumDefinition::Ptr def = mEnums.at(i);
        if (def->name() == defName)
        {
            kDebug()
                << "found at index: " << i;
            return def;
        }
    }
    kDebug()
        << "enum " << defName << "not found in enums list";
    return EnumDefinition::Ptr(NULL);
}

void OsdParser::parseEnumDefNodes(QDomNodeList& elems)
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
        PrimitiveDataType type = PrimitiveFactory::typeStringToType(typeStr);
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
}

#if 0
void OsdParser::parseIncludeNodes(QDomNodeList& elems)
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
#endif
