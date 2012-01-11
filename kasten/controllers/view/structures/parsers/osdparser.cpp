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

#include "osdparser.h"

#include "../datatypes/array/arraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/primitive/enumdatainformation.h"
#include "../datatypes/primitive/flagdatainformation.h"
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
#include <QScriptEngine>

OsdParser::OsdParser(const Kasten2::StructureDefinitionFile* const def) :
    AbstractStructureParser(def), mDir(def->dir()), mEngine(0), mEnumsParsed(false), mFullyParsed(false)
{
    mFile = QString(def->pluginInfo().pluginName() + QLatin1String(".osd"));
    openDocFromFile();
}

OsdParser::OsdParser(const QString dir, const QString file) :
    AbstractStructureParser(0), mDir(dir), mFile(file), mEngine(0), mEnumsParsed(false), mFullyParsed(false)
{
    openDocFromFile();
}

OsdParser::OsdParser(const QString xml) :
    AbstractStructureParser(0), mEngine(0), mEnumsParsed(false), mFullyParsed(false)
{
    int errorLine, errorColumn;
    QString errorMsg;
    if (!mDocument.setContent(xml, false, &errorMsg, &errorLine, &errorColumn))
    {
        kWarning() << "error reading xml:\n" << errorMsg << "\n error line="
            << errorLine << " error column=" << errorColumn;
        kDebug() << "XML was: " << xml;
    }
}

OsdParser::~OsdParser()
{
    //don't delete mEngine, ownership gets taken over
}

QStringList OsdParser::parseStructureNames()
{
    QStringList ret;
    QDomNode rootElem = mDocument.firstChildElement(QLatin1String("data"));
    QDomNodeList nodes = rootElem.childNodes();
    for (uint i = 0; i < nodes.length(); ++i)
    {
        QDomElement elem = nodes.at(i).toElement(); // try to convert the node to an element.
        if (!elem.isNull())
        {
            //      kDebug() << "element tag: " << elem.tagName();
            //e is element
            QString tag = elem.tagName();
            if (tag == QLatin1String("struct")
                || tag == QLatin1String("array")
                || tag == QLatin1String("bitfield")
                || tag == QLatin1String("primitive")
                || tag == QLatin1String("union")
                || tag == QLatin1String("enum")
                || tag == QLatin1String("flags")
                || tag == QLatin1String("string"))
                ret.append(elem.attribute(QLatin1String("name"), QLatin1String("<invalid name>")));
        }
    }
    return ret;
}

void OsdParser::openDocFromFile()
{
    QFileInfo fileInfo(mDir, mFile);
    if (!fileInfo.exists())
    {
        kWarning() << "Structure definition " << fileInfo.absoluteFilePath() << " does not exist";
        return;
    }
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        kWarning() << "could not open file " << fileInfo.absoluteFilePath();
        return;
    }
    int errorLine, errorColumn;
    QString errorMsg;
    if (!mDocument.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        kWarning() << "error reading file " << mFile << ":\n" << errorMsg << "\n error line="
            << errorLine << " error column=" << errorColumn;
    }
    file.close();
}

QList<const TopLevelDataInformation*> OsdParser::parseStructures()
{
    QFileInfo fileInfo(mDir, mFile);
    QDomElement rootElem = mDocument.firstChildElement(QLatin1String("data"));

    parseEnums();

    QList<const TopLevelDataInformation*> structures;
    QDomNodeList list = rootElem.childNodes();
    for (uint i = 0; i < list.length(); ++i)
    {
        const QDomNode& n = list.at(i);
        QDomElement elem = n.toElement(); // try to convert the node to an element.
        if (elem.isNull())
            continue;

        //e is element
        //kDebug() << "element tag: " << elem.tagName();
        QString tag = elem.tagName();
        DataInformation* data = parseNode(elem, 0);
        if (data)
        {
            TopLevelDataInformation* topData = new TopLevelDataInformation(data, fileInfo, mEngine, false);
            structures.append(topData);
        }
        else
        {
            kDebug() << "data == NULL -> could not parse node " << elem.tagName();
            delete data;
        }
    }
    mFullyParsed = true; //however if this method is called again the result will not be cached
    return structures;
}

bool OsdParser::isFullyParsed() const
{
    return mFullyParsed;
}

void OsdParser::parseEnums()
{
    if (mEnumsParsed)
        return;
    QDomElement rootElem = mDocument.firstChildElement(QLatin1String("data"));
    QDomNodeList enumDefs = rootElem.elementsByTagName(QLatin1String("enumDef"));
    parseEnumDefNodes(enumDefs);
    mEnumsParsed = true;
}

//Datatypes

ArrayDataInformation* OsdParser::arrayFromXML(const QDomElement& xmlElem, const DataInformation* parent)
{
    QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    QDomNode node = xmlElem.firstChild();
    QString lengthStr = xmlElem.attribute(QLatin1String("length"));
    if (lengthStr.isNull())
    {
        kWarning() << name << ": no length attribute defined";
        return 0;
    }
    ArrayDataInformation* retVal;
    QScriptValue updateFunc;
    bool okay = true;
    int length = lengthStr.toInt(&okay, 10);
    if (length < 0)
    {
        kWarning() << "parsing "<< name << ": length is < 0 (" << length << ")";
        return 0;
    }
    if (!okay)
    {
        length = 0;
        QString access;
        kDebug() << "error parsing length string -> is dynamic length array. Length string=" << lengthStr;
        //we have to find an element that matches the element passed in lengthStr
        const DataInformation* currElem = parent;
        if (!currElem) {
            kWarning() << name << ": array without length/length depending on other member given as root element of .osd";
            return 0;
        }
        if (lengthStr.contains(QLatin1Char('.')))
        {
            //absolute 'path' to element given, i.e. data.elem1.arr_length
            QStringList elements = lengthStr.split(QLatin1Char('.'), QString::SkipEmptyParts);
            for (int i = 0; i < elements.size(); ++i) {
                DataInformation* child = currElem->child(elements.at(i));
                if (child)
                    currElem = child;
                else
                {
                    kDebug() << name << ": could not find a child with name " << lengthStr;
                    return 0;
                }
            }
            access = QLatin1String("this.parent.") + lengthStr + QLatin1String(".value"); //was valid
        }
        else
        {
            QPair<DataInformation*, QString> tmp
                = parent->findChildForDynamicArrayLength(lengthStr, parent->childCount());
            if (!tmp.first)
            {
                kDebug() << "array " << name << ": could not find referenced element " << lengthStr;
                return 0;
            }
            kDebug() << name << ": update var = " << tmp.second;
            access = tmp.second;
        }
        if (!mEngine)
            mEngine = new QScriptEngine();
        QString script(QLatin1String("x = function() { this.length = this.parent.")
            + access + QLatin1String(";}"));
        updateFunc = mEngine->evaluate(script);
        //kDebug() << "update func = :" << script;
        Q_ASSERT(updateFunc.isFunction());
        //parse subelement
    }
    DataInformation* subElem = parseNode(node, parent);
    if (!subElem)
    {
        kWarning() << name << ": could not parse subelement type";
        return 0;
    }
    retVal = new ArrayDataInformation(name, length, subElem);
    if (updateFunc.isValid())
        retVal->setAdditionalData(new AdditionalData(QScriptValue(), updateFunc));
    return retVal;
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem)
{
    QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute(QLatin1String("type"), QString());
    if (typeStr.isEmpty())
    {
        kWarning() << "no type attribute defined for primitive type" << name;
        return 0;
    }
    return PrimitiveFactory::newInstance(name, typeStr);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(const QDomElement& xmlElem)
{
    kDebug() << "loading bitfield";
    QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute(QLatin1String("type"), QString());
    QString widthStr = xmlElem.attribute(QLatin1String("width"), QString());
    bool okay = false;
    uint width = widthStr.toUInt(&okay, 10);
    if (width == 0 || !okay)
    {
        kWarning() << "invalid width specified (width must be > 0)";
        return NULL;
    }
    AbstractBitfieldDataInformation* bitf = 0;
    if (typeStr == QLatin1String("bool"))
        bitf = new BoolBitfieldDataInformation(name, width);
    else if (typeStr == QLatin1String("unsigned"))
        bitf = new UnsignedBitfieldDataInformation(name, width);
    else if (typeStr == QLatin1String("signed"))
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
    QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    UnionDataInformation* un = new UnionDataInformation(name);
    QDomNode node = xmlElem.firstChild();
    while (!node.isNull())
    {
        DataInformation* data = parseNode(node, un);
        if (data)
            un->addDataTypeToUnion(data);
        node = node.nextSibling();
    }
    return un;
}

StructureDataInformation* OsdParser::structFromXML(const QDomElement& xmlElem)
{
    QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    StructureDataInformation* stru = new StructureDataInformation(name);
    QDomNode node = xmlElem.firstChild();
    while (!node.isNull())
    {
        DataInformation* data = parseNode(node, stru);
        if (data)
            stru->addDataTypeToStruct(data);
        node = node.nextSibling();
    }
    return stru;
}

AbstractEnumDataInformation* OsdParser::enumFromXML(const QDomElement& xmlElem, bool isFlags)
{
    if (!mEnumsParsed) //not always needed
        parseEnums();

    QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    QString typeStr = xmlElem.attribute(QLatin1String("type"), QString());
    if (typeStr.isEmpty())
    {
        kWarning() << "no type attribute defined";
        return NULL;
    }
    QString enumName = xmlElem.attribute(QLatin1String("enum"), QString());
    if (enumName.isEmpty())
    {
        kWarning() << "no enum attribute defined";
        return NULL;
    }
    EnumDefinition::Ptr def = findEnum(enumName);
    if (!def)
    {
        kWarning() << "no enum with name " << enumName << "found.";
        return NULL;
    }
    PrimitiveDataInformation* prim = PrimitiveFactory::newInstance(name, typeStr);
    if (!prim)
    {
        kWarning() << "primitive type is null!!";
        return NULL;
    }

    if (isFlags)
       return new FlagDataInformation(name, prim, def);
    else
        return new EnumDataInformation(name, prim, def);
}

StringDataInformation* OsdParser::stringFromXML(const QDomElement& node)
{
    const QString name = node.attribute(QLatin1String("name"), i18n("<invalid name>"));
    const QString terminatedBy = node.attribute(QLatin1String("terminatedBy"));
    const QString charCount = node.attribute(QLatin1String("maxCharCount"));
    const QString byteCount = node.attribute(QLatin1String("maxByteCount"));
    const QString encoding = node.attribute(QLatin1String("encoding"));

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

DataInformation* OsdParser::parseNode(const QDomNode& node, const DataInformation* parent)
{
    QDomElement elem = node.toElement(); // try to convert the node to an element.
    DataInformation* data = NULL;
    if (!elem.isNull())
    {
        //      kDebug() << "element tag: " << elem.tagName();
        //e is element
        const QString tag = elem.tagName();
        if (tag == QLatin1String("struct"))
            data = structFromXML(elem);
        else if (tag == QLatin1String("array"))
            data = arrayFromXML(elem, parent);
        else if (tag == QLatin1String("bitfield"))
            data = bitfieldFromXML(elem);
        else if (tag == QLatin1String("primitive"))
            data = primitiveFromXML(elem);
        else if (tag == QLatin1String("union"))
            data = unionFromXML(elem);
        else if (tag == QLatin1String("enum"))
            data = enumFromXML(elem, false);
        else if (tag == QLatin1String("flags"))
            data = enumFromXML(elem, true);
        else if (tag == QLatin1String("string"))
            data = stringFromXML(elem);
    }
    if (data) {
        QString byteOrder = elem.attribute(QLatin1String("byteOrder"), QLatin1String("inherit"));
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
            kDebug() << "found at index: " << i;
            return def;
        }
    }
    kDebug() << "enum " << defName << "not found in enums list";
    return EnumDefinition::Ptr(0);
}

void OsdParser::parseEnumDefNodes(QDomNodeList& elems)
{
    for (uint i = 0; i < elems.length(); i++)
    {
        QDomElement elem = elems.at(i).toElement();
        if (elem.isNull())
            continue;
        QMap<AllPrimitiveTypes, QString> defs;
        QString enumName = elem.attribute(QLatin1String("name"), i18n("<no name specified>"));
        QString typeStr = elem.attribute(QLatin1String("type"));
        if (typeStr.isNull())
        {
            kWarning() << "no type attribute defined -> skipping this enum";
            continue;
        }
        PrimitiveDataType type = PrimitiveFactory::typeStringToType(typeStr);
        //handle all entries
        QDomNodeList children = elem.elementsByTagName(QLatin1String("entry"));
        for (uint j = 0; j < children.length(); j++)
        {
            QDomElement child = children.at(j).toElement();
            if (child.isNull())
                continue;
            QString name = child.attribute(QLatin1String("name"), i18n("<no name specified>"));
            bool okay = false;
            QString valStr = child.attribute(QLatin1String("value"), QString());
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
