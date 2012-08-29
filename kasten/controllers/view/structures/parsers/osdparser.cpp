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

#include "osdparser.h"

#include "../datatypes/array/arraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/primitive/enumdatainformation.h"
#include "../datatypes/primitive/flagdatainformation.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../datatypes/strings/stringdata.h"
#include "../datatypes/primitivefactory.h"
#include "../datatypes/topleveldatainformation.h"
#include "../datatypes/dummydatainformation.h"
#include "../datatypes/datainformationfactory.h"
#include "../structuredefinitionfile.h"
#include "../script/scriptlogger.h"
#include "../script/scriptengineinitializer.h"

#include <QFile>
#include <QFileInfo>

#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QScriptEngine>

#include <KDebug> //FIXME remove this!!

//FIXME remove all kdebug & kwarning in here!
OsdParser::OsdParser(const QString& pluginName, const QString& absolutePath)
        : AbstractStructureParser(pluginName, absolutePath)
{
}

OsdParser::OsdParser(const QString& xml)
        : AbstractStructureParser(QString(), QString()), mXmlString(xml)
{
}

OsdParser::~OsdParser()
{
}

QDomDocument OsdParser::openDocFromString(ScriptLogger * logger) const
{
    Q_ASSERT(!mXmlString.isEmpty());
    int errorLine, errorColumn;
    QString errorMsg;
    QDomDocument doc;
    if (!doc.setContent(mXmlString, false, &errorMsg, &errorLine, &errorColumn))
    {
        const QString errorOutput = QString(
                QLatin1String("error reading XML: %1\n error line=%2\nerror column=%3"))
                .arg(errorMsg, QString::number(errorLine), QString::number(errorColumn));
        if (logger)
        {
            logger->error() << errorOutput;
            logger->info() << "XML was:" << mXmlString;
        }
        else
        {
            kWarning() << errorOutput;
            kWarning() << "XML was:" << mXmlString;
        }
        return QDomDocument();
    }
    return doc;
}

QDomDocument OsdParser::openDocFromFile(ScriptLogger* logger) const
{
    QFileInfo fileInfo(mAbsolutePath);
    if (!fileInfo.exists())
    {
        logger->error() << "File" << mAbsolutePath << "does not exist!";
        return QDomDocument();
    }
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        const QString errorOutput = QLatin1String("Could not open file ") + mAbsolutePath;
        logger->error() << errorOutput;
        return QDomDocument();
    }
    int errorLine, errorColumn;
    QString errorMsg;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        const QString errorOutput = QString(
                QLatin1String("error reading XML: %1\n error line=%2\nerror column=%3"))
                .arg(errorMsg, QString::number(errorLine), QString::number(errorColumn));
        logger->error() << errorOutput;
        logger->info() << "File was:" << mAbsolutePath;
    }
    file.close();
    return doc;
}

QStringList OsdParser::parseStructureNames() const
{
    QStringList ret;
    QScopedPointer<ScriptLogger> rootLogger(new ScriptLogger); //only needed in we get an error right now
    QDomDocument document = openDoc(rootLogger.data());
    if (document.isNull())
        return QStringList();
    QDomNode rootElem = document.firstChildElement(QLatin1String("data"));
    if (rootElem.isNull())
        return QStringList();
    QDomNodeList nodes = rootElem.childNodes();
    for (uint i = 0; i < nodes.length(); ++i)
    {
        QDomElement elem = nodes.at(i).toElement(); // try to convert the node to an element.
        if (!elem.isNull())
        {
            QString tag = elem.tagName();
            if (tag == QLatin1String("struct") || tag == QLatin1String("array")
                    || tag == QLatin1String("bitfield") || tag == QLatin1String("primitive")
                    || tag == QLatin1String("union") || tag == QLatin1String("enum")
                    || tag == QLatin1String("flags") || tag == QLatin1String("string"))
            {
                ret.append(elem.attribute(QLatin1String("name"), QLatin1String("<invalid name>")));
            }
            else
            {
                kWarning().nospace() << "Unknown tag name in plugin " << mPluginName << " :" << tag;
            }
        }
    }
    //print the errors to stderr, since we the logger will not be visible in the script console
    QStringList warnings = rootLogger->messages(ScriptLogger::LogWarning);
    for (int i = 0; i < warnings.size(); ++i)
        kWarning() << warnings.at(i);
    return ret;
}

QVector<TopLevelDataInformation*> OsdParser::parseStructures() const
{
    QFileInfo fileInfo(mAbsolutePath);

    QVector<TopLevelDataInformation*> structures;
    QScopedPointer<ScriptLogger> rootLogger(new ScriptLogger()); //only needed in we get an error right now
    QDomDocument document = openDoc(rootLogger.data());
    if (document.isNull())
    {
        structures.append(new TopLevelDataInformation(
                new DummyDataInformation(0, fileInfo.fileName()), rootLogger.take(), 0, fileInfo));
        return structures;
    }

    QDomElement rootElem = document.firstChildElement(QLatin1String("data"));
    if (rootElem.isNull())
    {
        rootLogger->error() << "Missing top level <data> element!";
        structures.append(new TopLevelDataInformation(
                new DummyDataInformation(0, fileInfo.fileName()), rootLogger.take(), 0, fileInfo));
        return structures;
    }
    QDomNodeList list = rootElem.childNodes();

    for (uint i = 0; i < list.length(); ++i)
    {
        const QDomNode& n = list.at(i);
        const QDomElement elem = n.toElement(); // try to convert the node to an element.
        if (elem.isNull())
            continue;

        //e is element
        if (elem.tagName() == QLatin1String("enumDef"))
            continue; //skip enum defs

        QScriptEngine* eng = ScriptEngineInitializer::newEngine(); //we need this for dynamic arrays
        ScriptLogger* logger = new ScriptLogger();
        QVector<EnumDefinition::Ptr> enums = parseEnums(rootElem, logger);
        OsdParserInfo info(QString(), logger, 0, eng, enums);

        DataInformation* data = parseNode(elem, info);

        if (!data)
        {
            logger->error() << "Failed to parse:" << toRawXML(elem);
            data = new DummyDataInformation(0,
                    fileInfo.absoluteFilePath() + QLatin1String("_element") + QString::number(i));
        }
        //if we don't need the engine, there is no point in wasting memory for it
        if (!info.scriptEngineNeeded)
        {
            delete eng;
            eng = 0;
        }
        TopLevelDataInformation* topData = new TopLevelDataInformation(data, logger, eng, fileInfo);
        structures.append(topData);
    }
    return structures;
}

QVector<EnumDefinition::Ptr> OsdParser::parseEnums(const QDomElement& rootElem, ScriptLogger* logger) const
{
    QVector<EnumDefinition::Ptr> ret;
    QDomNodeList enumDefs = rootElem.elementsByTagName(QLatin1String("enumDef"));
    for (uint i = 0; i < enumDefs.length(); i++)
    {
        const QDomElement elem = enumDefs.at(i).toElement();
        if (elem.isNull())
            continue;
        QMap<AllPrimitiveTypes, QString> defs;
        const QString enumName = elem.attribute(QLatin1String("name"), i18n("<no name specified>"));
        const QString typeStr = elem.attribute(QLatin1String("type"));
        if (typeStr.isEmpty())
        {
            logger->error() << "Skipping enum definition, since no type "
                    "attribute was found: " << toRawXML(elem);
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
            QString value = child.attribute(QLatin1String("value"), QString());
            QPair<AllPrimitiveTypes, QString> converted =
                    AbstractEnumDataInformation::convertToEnumEntry(name, value, logger, type, QString());
            if (converted == QPair<AllPrimitiveTypes, QString>())
                continue;
            defs.insert(converted.first, converted.second);
        }
        //now add this enum to the list of enums
        if (defs.isEmpty())
        {
            logger->error() << "enum definition with name " << enumName
                    << " contains no valid elements, skipping it\nError in: "
                    << toRawXML(elem);
        }
        else
        {
            EnumDefinition::Ptr enumDef = EnumDefinition::Ptr(new EnumDefinition(defs, enumName, type));
            ret.append(enumDef);
        }
    }
    return ret;
}

//Datatypes

ArrayDataInformation* OsdParser::arrayFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const
{
    QString name = xmlElem.attribute(QLatin1String("name"));
    QDomNode node = xmlElem.firstChild();
    QString lengthStr = xmlElem.attribute(QLatin1String("length"));
    if (lengthStr.isNull())
    {
        info.logger->error() << "No length attribute specified!\n In element "
                << toRawXML(xmlElem);
        return 0;
    }
    QScriptValue updateFunc;
    bool okay = true;
    int length = lengthStr.toInt(&okay, 10);
    if (okay)
    {
        if (length < 0)
        {
            info.logger->error()
            << QString(QLatin1String("Length is less than 0 (%1).\nIn element %2"))
                    .arg(QString::number(length), toRawXML(xmlElem));
            return 0;
        }
    }
    else
    {
        length = 0; //just to be safe
        //could not parse as number -> must be a member (dynamic array)
        QString access;
        //we have to find an element that matches the element passed in lengthStr
        const DataInformation* currElem = info.parent;
        if (!currElem)
        {
            info.logger->error() << "Array with length depending on other"
                    " member given as root element: " << toRawXML(xmlElem);
            return 0;
        }
        if (lengthStr.contains(QLatin1Char('.')))
        {
            //absolute 'path' to element given, i.e. data.elem1.arr_length
            QStringList elements = lengthStr.split(QLatin1Char('.'), QString::SkipEmptyParts);
            for (int i = 0; i < elements.size(); ++i)
            {
                DataInformation* child = currElem->child(elements.at(i));
                if (child)
                    currElem = child;
                else
                {
                    info.logger->error().nospace() << "could not find a child with name '"
                            << lengthStr << "'.\nIn element " << toRawXML(xmlElem);
                    return 0;
                }
            }
            access = lengthStr + QLatin1String(".value");
            if (!lengthStr.startsWith(QLatin1String("this.")))
                access = QLatin1String("mainStruct.") + access;
        }
        else
        {
            QPair<DataInformation*, QString> tmp
            = currElem->findChildForDynamicArrayLength(lengthStr, currElem->childCount());
            if (!tmp.first)
            {
                info.logger->error().nospace() << "could not find referenced element '"
                        << lengthStr << "'.\nIn element " << toRawXML(xmlElem);
                return 0;
            }
            access = tmp.second;
        }
        QString script(QLatin1String("var x = function(mainStruct) { this.length = ")
                + access + QLatin1String(";};x"));
        updateFunc = info.engine->evaluate(script);
        info.logger->info() << "setting updateFunc for array to" << script;
        Q_ASSERT(updateFunc.isFunction());
        info.scriptEngineNeeded = true;
    }
    //parse subelement
    //use dummy until real type has been determined
    ArrayDataInformation* tmp = new ArrayDataInformation(name, 0,
            new DummyDataInformation(0, QString()), info.parent);
    OsdParserInfo newInfo = info;
    newInfo.parent = tmp;
    DataInformation* subElem = parseNode(node, newInfo);
    delete tmp;
    if (newInfo.scriptEngineNeeded)
        info.scriptEngineNeeded = true; //we need it since child needs it

    if (!subElem)
    {
        info.logger->error() << "could not parse subelement type of array:" << toRawXML(xmlElem);
        return 0;
    }
    Q_ASSERT(!subElem->isDummy());
    ArrayDataInformation* retVal = new ArrayDataInformation(name, length, subElem, info.parent);
    if (updateFunc.isValid())
        retVal->setUpdateFunc(updateFunc);

    return retVal;
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const
{
    PrimitiveParsedData ppd(info);
    ppd.type = xmlElem.attribute(QLatin1String("type"));
    return DataInformationFactory::newPrimitive(ppd);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const
{
    BitfieldParsedData bpd(info);
    bpd.type = xmlElem.attribute(QLatin1String("type"));
    bpd.widthStr = xmlElem.attribute(QLatin1String("width"));
    bpd.width = bpd.widthStr.toInt(&bpd.widthConversionOkay, 10);
    return DataInformationFactory::newBitfield(bpd);
}

template<class T>
T* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const
{
    T* structOrUnion = new T(info.name, info.parent);
    QDomNode node = xmlElem.firstChild();
    QVector<DataInformation*> children;
    OsdParserInfo newInfo(info);
    newInfo.parent = structOrUnion;
    while (!node.isNull())
    {
        //parseNode sets the name for us
        DataInformation* data = parseNode(node, newInfo);
        if (data)
            children.append(data);
        node = node.nextSibling();
    }
    structOrUnion->setInitialChildren(children);
    if (newInfo.scriptEngineNeeded)
        info.scriptEngineNeeded = true;
    return structOrUnion;
}

//instantiate the template for both cases so we don't get linker errors
template UnionDataInformation* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const;
template StructureDataInformation* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem, OsdParserInfo& info) const;

AbstractEnumDataInformation* OsdParser::enumFromXML(const QDomElement& xmlElem, bool isFlags,
        const OsdParserInfo& info) const
{
    EnumParsedData epd(info);
    epd.type = xmlElem.attribute(QLatin1String("type"));
    epd.enumName = xmlElem.attribute(QLatin1String("enum"));
    epd.enumDef = findEnum(epd.enumName, info);
    if (!epd.enumDef)
    {
        info.error().nospace() << "Enum definition '" << epd.enumName << "' does not exist!";
        return 0;
    }

    if (isFlags)
        return DataInformationFactory::newFlags(epd);
    else
        return DataInformationFactory::newEnum(epd);
}

StringDataInformation* OsdParser::stringFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const
{
    const QString name = xmlElem.attribute(QLatin1String("name"), i18n("<invalid name>"));
    const QString terminatedBy = xmlElem.attribute(QLatin1String("terminatedBy"));
    const QString charCount = xmlElem.attribute(QLatin1String("maxCharCount"));
    const QString byteCount = xmlElem.attribute(QLatin1String("maxByteCount"));
    const QString encoding = xmlElem.attribute(QLatin1String("encoding"));

    StringData::TerminationModes mode = StringData::None;
    if (!terminatedBy.isEmpty())
        mode |= StringData::Sequence;
    if (!charCount.isEmpty())
        mode |= StringData::CharCount;
    if (!byteCount.isEmpty())
        mode |= StringData::ByteCount;

    if ((mode & StringData::CharCount) && (mode & StringData::ByteCount))
        mode &= ~StringData::ByteCount; //when both exists charcount wins

    StringDataInformation* data = new StringDataInformation(name, encoding, info.parent);
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
            kDebug()
            << "invalid termination codepoint specified " << terminatedBy
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
            kDebug()
            << "unparseable char count: " << charCount << ", defaulting to 1";
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
            kDebug() << "unparseable byte count: " << byteCount << ", defaulting to 1";
            count = 1;
        }
        data->setMaxByteCount(count);
    }
    return data;
}

DataInformation* OsdParser::parseNode(const QDomNode& node, OsdParserInfo& info) const
{
    QDomElement elem = node.toElement(); // try to convert the node to an element.
    DataInformation* data = 0;
    if (!elem.isNull())
    {
        //      kDebug() << "element tag: " << elem.tagName();
        //e is element
        const QString tag = elem.tagName();
        info.name = elem.attribute(QLatin1String("name"), i18n("&lt;invalid name&gt;"));
        if (tag == QLatin1String("struct"))
            data = structFromXML(elem, info);
        else if (tag == QLatin1String("array"))
            data = arrayFromXML(elem, info);
        else if (tag == QLatin1String("bitfield"))
            data = bitfieldFromXML(elem, info);
        else if (tag == QLatin1String("primitive"))
            data = primitiveFromXML(elem, info);
        else if (tag == QLatin1String("union"))
            data = unionFromXML(elem, info);
        else if (tag == QLatin1String("enum"))
            data = enumFromXML(elem, false, info);
        else if (tag == QLatin1String("flags"))
            data = enumFromXML(elem, true, info);
        else if (tag == QLatin1String("string"))
            data = stringFromXML(elem, info);
    }
    if (data)
    {
        QString byteOrder = elem.attribute(QLatin1String("byteOrder"),
                QLatin1String("inherit"));
        data->setByteOrder(byteOrderFromString(byteOrder, info.logger));
    }
    return data;
}

EnumDefinition::Ptr OsdParser::findEnum(const QString& defName, const OsdParserInfo& info) const
{
    for (int i = 0; i < info.enums.size(); ++i)
    {
        const EnumDefinition::Ptr def = info.enums.at(i);
        if (def->name() == defName)
            return def;
    }
    info.logger->error(info.parent ? info.parent->fullObjectPath() : QString())
            << "Could not find enum definition with name" << defName;
    return EnumDefinition::Ptr(0);
}

QString OsdParser::toRawXML(const QDomElement& elem) const
{
    QString ret = QLatin1Char('<') + elem.tagName();
    QDomNamedNodeMap attrs = elem.attributes();
    for (int i = 0; i < attrs.size(); ++i)
    {
        QDomAttr attr = attrs.item(i).toAttr();
        ret += QLatin1Char(' ') + attr.name() + QLatin1String("=\"") + attr.value()
                + QLatin1Char('"');
    }
    ret += QLatin1String("/>");
    return ret;
}
