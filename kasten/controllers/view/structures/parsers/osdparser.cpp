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

#include "datainformationfactory.h"
#include "../datatypes/array/arraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../datatypes/strings/stringdata.h"
#include "../datatypes/primitivefactory.h"
#include "../datatypes/topleveldatainformation.h"
#include "../datatypes/dummydatainformation.h"
#include "../structuredefinitionfile.h"
#include "../script/scriptlogger.h"
#include "../script/scriptengineinitializer.h"

#include <QFile>
#include <QFileInfo>

#include <QDomDocument>
#include <QDomElement>
#include <QScriptEngine>

using namespace ParserStrings;

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

QDomDocument OsdParser::openDoc(ScriptLogger* logger) const
        {
    return mXmlString.isEmpty() ? openDocFromFile(logger) : openDocFromString(logger);
}

QDomDocument OsdParser::openDocFromString(ScriptLogger * logger) const
{
    Q_CHECK_PTR(logger);
    Q_ASSERT(!mXmlString.isEmpty());
    int errorLine, errorColumn;
    QString errorMsg;
    QDomDocument doc;
    if (!doc.setContent(mXmlString, false, &errorMsg, &errorLine, &errorColumn))
    {
        const QString errorOutput = QString(
                QLatin1String("error reading XML: %1\n error line=%2\nerror column=%3"))
                .arg(errorMsg, QString::number(errorLine), QString::number(errorColumn));
        logger->error() << errorOutput;
        logger->info() << "XML was:" << mXmlString;

        return QDomDocument();
    }
    return doc;
}

QDomDocument OsdParser::openDocFromFile(ScriptLogger* logger) const
{
    Q_CHECK_PTR(logger);
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
    QScopedPointer<ScriptLogger> rootLogger(new ScriptLogger); //only needed if we get an error right now
    rootLogger->setLogToStdOut(true); //we cannot get our messages into the script console, so do this instead
    QDomDocument document = openDoc(rootLogger.data());
    if (document.isNull())
        return QStringList();
    QDomElement rootElem = document.firstChildElement(QLatin1String("data"));
    if (rootElem.isNull())
        return QStringList();
    for (QDomElement childElement = rootElem.firstChildElement();
            !childElement.isNull(); childElement = childElement.nextSiblingElement())
    {
        QString tag = childElement.tagName();
        if (tag == TYPE_STRUCT || tag == TYPE_ARRAY || tag == TYPE_BITFIELD || tag == TYPE_PRIMITIVE
                || tag == TYPE_UNION || tag == TYPE_ENUM || tag == TYPE_FLAGS || tag == TYPE_STRING)
        {
            //TODO allow e.g. <uint8 name="asfd">
            ret.append(readProperty(childElement, PROPERTY_NAME, i18n("&lt;invalid name&gt;")));
        }
        else
        {
            rootLogger->error(QString()).nospace() << "Unknown tag name in plugin " << mPluginName << " :"
                    << tag;
        }
    }
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
    int count = 1;
    for (QDomElement elem = rootElem.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        if (elem.tagName() == TYPE_ENUMDEF)
            continue; //skip enum defs

        QScriptEngine* eng = ScriptEngineInitializer::newEngine(); //we need this for dynamic arrays
        ScriptLogger* logger = new ScriptLogger();
        QVector<EnumDefinition::Ptr> enums = parseEnums(rootElem, logger);
        OsdParserInfo info(QString(), logger, 0, eng, enums);

        DataInformation* data = parseElement(elem, info);

        if (!data)
        {
            logger->error() << "Failed to parse:" << toRawXML(elem);
            data = new DummyDataInformation(0,
                    fileInfo.absoluteFilePath() + QLatin1String("_element") + QString::number(count));
        }
        TopLevelDataInformation* topData = new TopLevelDataInformation(data, logger, eng, fileInfo);
        structures.append(topData);
        count++;
    }
    return structures;
}

//TODO make type depend on the user not the definition
QVector<EnumDefinition::Ptr> OsdParser::parseEnums(const QDomElement& rootElem, ScriptLogger* logger) const
{
    QVector<EnumDefinition::Ptr> ret;
    for (QDomElement elem = rootElem.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        if (elem.tagName() != TYPE_ENUMDEF)
            continue;

        QMap<AllPrimitiveTypes, QString> defs;
        const QString enumName = readProperty(elem, PROPERTY_NAME, i18n("&lt;invalid name&gt;"));
        const QString typeStr = readProperty(elem, QLatin1String("type"));
        if (typeStr.isEmpty())
        {
            logger->error(enumName) << "Skipping enum definition, since no type attribute was found:"
                    << toRawXML(elem);
            continue;
        }
        PrimitiveDataType type = PrimitiveFactory::typeStringToType(typeStr);
        //handle all entries
        for (QDomElement child = elem.firstChildElement(); !child.isNull(); child =
                child.nextSiblingElement())
        {
            if (child.tagName() != QLatin1String("entry"))
                continue;

            QString name = readProperty(child, PROPERTY_NAME);
            if (name.isEmpty())
            {
                logger->warn(enumName) << "Entry is missing name, skipping it!";
                continue;
            }
            QString value = readProperty(child, PROPERTY_VALUE);
            QPair<AllPrimitiveTypes, QString> converted =
                    AbstractEnumDataInformation::convertToEnumEntry(name, value, logger, type, QString());
            if (converted == QPair<AllPrimitiveTypes, QString>())
                continue;
            defs.insert(converted.first, converted.second);
        }
        //now add this enum to the list of enums
        if (defs.isEmpty())
        {
            logger->warn(enumName) << "Enum definition contains no valid elements!";
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

ArrayDataInformation* OsdParser::arrayFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const
{
    ArrayParsedData apd(info);
    QString lengthStr = readProperty(xmlElem, PROPERTY_LENGTH);
    if (lengthStr.isEmpty())
    {
        info.error() << "No array length specified!";
        return 0;
    }

    apd.length = ParserUtils::intFromString(lengthStr);

    if (!apd.length.isValid)
    {
        //we failed to parse as an integer, must be an update function now
        //must wrap in parentheses, see https://bugreports.qt-project.org/browse/QTBUG-5757
        apd.lengthFunction = info.engine->evaluate(QLatin1Char('(') + lengthStr + QLatin1Char(')'));
        if (!apd.lengthFunction.isFunction())
        {
            info.error() << "Array length is neither number nor function ('" << lengthStr
                    << "').\nMaybe this structure uses the old way of defining dynamic arrays.\n"
                            "To fix this replace it with e.g.\n"
                            "function() { return this.parent.LengthField.value; }";
            return 0;
        }
    }

    //first check whether there is a <type> element and use the inner element
    //if that doesn't exist use the first child element as the type, but only if there is only one child
    QDomElement childElement = xmlElem.firstChildElement(PROPERTY_TYPE).firstChildElement();
    if (childElement.isNull())
    {
        childElement = xmlElem.firstChildElement();
        if (childElement.isNull())
        {
            info.error() << "Array type is missing! Please add a &lt;type&gt; child element.";
            return 0;
        }
        else if (childElement != xmlElem.lastChildElement())
        {
            //there is more than one child element
            info.error().nospace() << "There is more than one child element, cannot determine which one "
                    "wrap the correct one is the array type. Wrap the correct one in a <type> element.";
            return 0;
        }
    }
    OsdParserInfo newInfo = info;
    newInfo.parent = 0;
    newInfo.name = info.context();
    apd.arrayType = parseElement(childElement, newInfo);
    return DataInformationFactory::newArray(apd);
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const
{
    PrimitiveParsedData ppd(info);
    ppd.type = readProperty(xmlElem, PROPERTY_TYPE);
    return DataInformationFactory::newPrimitive(ppd);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const
{
    BitfieldParsedData bpd(info);
    bpd.type = readProperty(xmlElem, PROPERTY_TYPE);
    QString width = readProperty(xmlElem, PROPERTY_WIDTH);
    bpd.width = ParserUtils::intFromString(width);
    return DataInformationFactory::newBitfield(bpd);
}

template<class T>
T* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const
{
    QVector<DataInformation*> children;

    DummyDataInformation dummy(0, info.context());
    OsdParserInfo newInfo(info);
    newInfo.parent = &dummy;
    for (QDomElement elem = xmlElem.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        DataInformation* data = parseElement(elem, newInfo);
        if (data)
            children.append(data);
    }
    T* structOrUnion = new T(info.name, children, info.parent);
    return structOrUnion;
}

//instantiate the template for both cases so we don't get linker errors
template UnionDataInformation* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const;
template StructureDataInformation* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const;

AbstractEnumDataInformation* OsdParser::enumFromXML(const QDomElement& xmlElem, bool isFlags,
        const OsdParserInfo& info) const
{
    EnumParsedData epd(info);
    epd.type = readProperty(xmlElem, PROPERTY_TYPE);
    epd.enumName = readProperty(xmlElem, TYPE_ENUM); //used again here as property
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
    //TODO clean up this code
    const QString terminatedBy = readProperty(xmlElem, PROPERTY_TERMINATED_BY);
    const QString charCount = readProperty(xmlElem, PROPERTY_MAX_CHAR_COUNT);
    const QString byteCount = readProperty(xmlElem, PROPERTY_MAX_BYTE_COUNT);
    const QString encoding = readProperty(xmlElem, PROPERTY_ENCODING);

    StringData::TerminationModes mode = StringData::None;
    if (!terminatedBy.isEmpty())
        mode |= StringData::Sequence;
    if (!charCount.isEmpty())
        mode |= StringData::CharCount;
    if (!byteCount.isEmpty())
        mode |= StringData::ByteCount;

    if ((mode & StringData::CharCount) && (mode & StringData::ByteCount))
        mode &= ~StringData::ByteCount; //when both exists charcount wins

    StringDataInformation* data = new StringDataInformation(info.name, encoding, info.parent);
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
            kDebug()
            << "unparseable byte count: " << byteCount << ", defaulting to 1";
            count = 1;
        }
        data->setMaxByteCount(count);
    }
    return data;
}

DataInformation* OsdParser::parseElement(const QDomElement& elem, const OsdParserInfo& oldInfo) const
{
    Q_ASSERT(!elem.isNull());
    DataInformation* data = 0;
    //kDebug() << "element tag: " << elem.tagName();
    const QString tag = elem.tagName();
    OsdParserInfo info(oldInfo);
    info.name = readProperty(elem, PROPERTY_NAME, i18n("&lt;invalid name&gt;"));
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

    if (data)
    {
        CommonParsedData cpd(oldInfo, oldInfo.engine);
        QString byteOrderStr = readProperty(elem, PROPERTY_BYTEORDER);
        if (!byteOrderStr.isEmpty())
            cpd.endianess = ParserUtils::byteOrderFromString(byteOrderStr, oldInfo);
        cpd.updateFuncString = readProperty(elem, PROPERTY_UPDATE_FUNC);
        cpd.validationFuncString = readProperty(elem, PROPERTY_VALIDATION_FUNC);
        if (!DataInformationFactory::commonInitialization(data, cpd))
        {
            delete data; //error message has already been logged
            return 0;
        }
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
    info.error() << "Could not find enum definition with name" << defName;
    return EnumDefinition::Ptr(0);
}

QString OsdParser::readProperty(const QDomElement& elem, const QString& property, const QString& defaultVal)
{
    const QString attrib = elem.attribute(property);
    if (!attrib.isEmpty())
        return attrib;
    //check for element now
    const QDomElement childElem = elem.firstChildElement(property);
    if (!elem.isNull())
        return elem.text();
    else
        return defaultVal;
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
