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
#include <QDomNode>
#include <QDomElement>
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
                ret.append(readProperty(elem, QLatin1String("name"), i18n("&lt;invalid name&gt;")));
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
        const QString enumName = readProperty(elem, QLatin1String("name"), i18n("&lt;invalid name&gt;"));
        const QString typeStr = readProperty(elem, QLatin1String("type"));
        if (typeStr.isEmpty())
        {
            logger->error() << "Skipping enum definition, since no type attribute was found:" << toRawXML(elem);
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
            QString name = readProperty(child, QLatin1String("name"), i18n("<no name specified>"));
            QString value = readProperty(child, QLatin1String("value"), QString());
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

ArrayDataInformation* OsdParser::arrayFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const
{
    ArrayParsedData apd(info);
    QString lengthStr = readProperty(xmlElem, QLatin1String("length"));
    if (lengthStr.isEmpty())
    {
        info.error() << "No array length specified!";
        return 0;
    }

    bool okay = true;
    if (lengthStr.startsWith(QLatin1String("0x")))
        apd.length = lengthStr.mid(2).toInt(&okay, 16);
    else
        apd.length = lengthStr.toInt(&okay, 10);

    if (!okay)
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
    QDomElement childElement = xmlElem.firstChildElement(QLatin1String("type")).firstChildElement();
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
    apd.arrayType = parseNode(childElement, newInfo);
    return DataInformationFactory::newArray(apd);
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const
{
    PrimitiveParsedData ppd(info);
    ppd.type = readProperty(xmlElem, QLatin1String("type"));
    return DataInformationFactory::newPrimitive(ppd);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info) const
{
    BitfieldParsedData bpd(info);
    bpd.type = readProperty(xmlElem, QLatin1String("type"));
    QString width = readProperty(xmlElem, QLatin1String("width"));
    bpd.width = ParserUtils::intFromString(width);
    return DataInformationFactory::newBitfield(bpd);
}

template<class T>
T* OsdParser::structOrUnionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info) const
{
    QDomNode node = xmlElem.firstChild();
    QVector<DataInformation*> children;

    DummyDataInformation dummy(0, info.context());
    OsdParserInfo newInfo(info);
    newInfo.parent = &dummy;
    while (!node.isNull())
    {
        DataInformation* data = parseNode(node, newInfo);
        if (data)
            children.append(data);
        node = node.nextSibling();
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
    epd.type = readProperty(xmlElem, QLatin1String("type"));
    epd.enumName = readProperty(xmlElem, QLatin1String("enum"));
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
    const QString name = readProperty(xmlElem, QLatin1String("name"), i18n("<invalid name>"));
    const QString terminatedBy = readProperty(xmlElem, QLatin1String("terminatedBy"));
    const QString charCount = readProperty(xmlElem, QLatin1String("maxCharCount"));
    const QString byteCount = readProperty(xmlElem, QLatin1String("maxByteCount"));
    const QString encoding = readProperty(xmlElem, QLatin1String("encoding"));

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
            kDebug()
            << "unparseable byte count: " << byteCount << ", defaulting to 1";
            count = 1;
        }
        data->setMaxByteCount(count);
    }
    return data;
}

DataInformation* OsdParser::parseNode(const QDomNode& node, const OsdParserInfo& oldInfo) const
{
    QDomElement elem = node.toElement(); // try to convert the node to an element.
    DataInformation* data = 0;
    if (!elem.isNull())
    {
        //      kDebug() << "element tag: " << elem.tagName();
        //e is element
        const QString tag = elem.tagName();
        OsdParserInfo info(oldInfo);
        info.name = readProperty(elem, QLatin1String("name"), i18n("&lt;invalid name&gt;"));
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
        CommonParsedData cpd(oldInfo, oldInfo.engine);
        QString byteOrderStr = readProperty(elem, QLatin1String("byteOrder"));
        if (!byteOrderStr.isEmpty())
            cpd.endianess = ParserUtils::byteOrderFromString(byteOrderStr, oldInfo);
        cpd.updateFuncString = readProperty(elem, QLatin1String("updateFunc"));
        cpd.validationFuncString = readProperty(elem, QLatin1String("validationFunc"));
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
