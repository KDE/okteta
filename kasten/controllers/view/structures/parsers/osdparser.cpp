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
#include <QScriptEngine>

using namespace ParserStrings;

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
QVector<EnumDefinition::Ptr> OsdParser::parseEnums(const QDomElement& rootElem, ScriptLogger* logger)
{
    QVector<EnumDefinition::Ptr> ret;
    for (QDomElement elem = rootElem.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        if (elem.tagName() != TYPE_ENUMDEF)
            continue;

        QMap<AllPrimitiveTypes, QString> defs;
        const QString enumName = readProperty(elem, PROPERTY_NAME, i18n("&lt;invalid name&gt;"));
        const QString typeStr = readProperty(elem, PROPERTY_TYPE);
        if (typeStr.isEmpty())
        {
            logger->error(enumName) << "Skipping enum definition, since no type attribute was found.";
            continue;
        }
        LoggerWithContext lwc(logger, QLatin1String("enum values (") + enumName + QLatin1Char(')'));
        PrimitiveDataType type = PrimitiveFactory::typeStringToType(typeStr, lwc);
        //handle all entries
        for (QDomElement child = elem.firstChildElement(); !child.isNull(); child =
                child.nextSiblingElement())
        {
            if (child.tagName() != QLatin1String("entry"))
                continue;

            QString name = readProperty(child, PROPERTY_NAME);
            if (name.isEmpty())
            {
                lwc.warn() << "Entry is missing name, skipping it!";
                continue;
            }
            QString value = readProperty(child, PROPERTY_VALUE);
            QPair<AllPrimitiveTypes, QString> converted =
                    EnumDefinition::convertToEnumEntry(name, value, lwc, type);
            if (converted == QPair<AllPrimitiveTypes, QString>())
                continue;
            defs.insert(converted.first, converted.second);
        }
        //now add this enum to the list of enums
        if (defs.isEmpty())
        {
            lwc.error() << "Enum definition contains no valid elements!";
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

ArrayDataInformation* OsdParser::arrayFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
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
        apd.lengthFunction = functionSafeEval(info.engine, lengthStr);
        if (!apd.lengthFunction.isFunction())
        {

            //it might be a simple reference (old way of defining dynamic arrays)
            if (!info.parent)
            {
                info.error() << "Toplevel array has length depending on other field (" << lengthStr
                        << "). This is not possible.";
                return 0;
            }
            if (lengthStr.contains(QLatin1Char('.')))
            {
                info.error() << "Referenced array length element (" << lengthStr << ") contains '.', this is allowed!";
                return 0; // XXX maybe add possible shorthand length="this.parent.length"
            }
            QString dynamicArray = generateLengthFunction(info.parent, 0, lengthStr, QString(), info);
            if (dynamicArray.isEmpty())
            {
                info.error() << "Could not find element " << lengthStr << " referenced as array length!";
                return 0;
            }

            apd.lengthFunction = functionSafeEval(info.engine, dynamicArray);
            if (!apd.lengthFunction.isFunction())
            {
                info.error() << "Array length is neither number nor valid function ('" << dynamicArray
                    << "'). This is probably a bug in the parser...";
                return 0;
            }
        }
    }

    //first check whether there is a <type> element and use the inner element
    //if that doesn't exist use the first child element as the type, but only if there is only one child
    DummyDataInformation dummy(info.parent, info.name); //dummy so that we have a proper chain
    QDomElement typeElement = xmlElem.firstChildElement(PROPERTY_TYPE).firstChildElement();
    if (typeElement.isNull())
    {
        //there is no <type> element, use first valid child element
        OsdChildrenParser typeParser(info, xmlElem.firstChildElement());
        typeParser.setParent(&dummy);
        if (typeParser.hasNext())
        {
            apd.arrayType = typeParser.next();
            if (typeParser.hasNext())
            {
                info.error() << "More than one possible type for array!";
                delete apd.arrayType;
                return 0;
            }
        }
        else
        {
            info.error() << "Array type is missing.";
            return 0;
        }
    }
    else
    {
        //we have a <type> element
        OsdParserInfo newInfo(info);
        newInfo.parent = &dummy;
        apd.arrayType = parseElement(typeElement, newInfo);
    }
    return DataInformationFactory::newArray(apd);
}

PointerDataInformation* OsdParser::pointerFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    PointerParsedData ppd(info);

    QDomElement typeElement = xmlElem.firstChildElement(PROPERTY_TYPE);
    QString typeString; //only needed if type is a string not an element

    OsdParserInfo newInfo(info);
    DummyDataInformation dummy(info.parent, info.name); //dummy so that we have a proper chain
    newInfo.parent = &dummy;
    newInfo.name = NAME_POINTER_VALUE_TYPE;

    if (!typeElement.isNull())
    {
        QDomElement toParse = typeElement.firstChildElement();
        if (!toParse.isNull())
        {
            ppd.valueType = parseElement(toParse, newInfo);
        }
        else
        {
            typeString = typeElement.text();
        }
    }
    else
    {
        typeString = xmlElem.attribute(PROPERTY_TYPE);
    }
    if (!typeString.isEmpty())
    {
        LoggerWithContext lwc(info.logger, info.context() + NAME_POINTER_VALUE_TYPE);
        ppd.valueType = PrimitiveFactory::newInstance(PROPERTY_TYPE, typeString, lwc);
    }

    //first check whether there is a <target> element and use the inner element
    //if that doesn't exist use the first child element as the type, but only if there is only one child
    QDomElement childElement = xmlElem.firstChildElement(PROPERTY_TARGET).firstChildElement();
    if (childElement.isNull())
    {
        childElement = xmlElem.firstChildElement();
        if (childElement.isNull())
        {
            info.error() << "Pointer target is missing! Please add a <target> child element.";
            return 0;
        }
        else if (childElement != xmlElem.lastChildElement())
        {
            //there is more than one child element
            info.error() << "There is more than one child element, cannot determine which one "
                    "is the pointer target. Wrap the correct one in a <target> element.";
            return 0;
        }
    }
    newInfo.name = NAME_POINTER_TARGET;
    ppd.pointerTarget = parseElement(childElement, newInfo);
    return DataInformationFactory::newPointer(ppd);
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    PrimitiveParsedData ppd(info);
    ppd.type = readProperty(xmlElem, PROPERTY_TYPE);
    return DataInformationFactory::newPrimitive(ppd);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info)
{
    BitfieldParsedData bpd(info);
    bpd.type = readProperty(xmlElem, PROPERTY_TYPE);
    QString width = readProperty(xmlElem, PROPERTY_WIDTH);
    bpd.width = ParserUtils::intFromString(width);
    return DataInformationFactory::newBitfield(bpd);
}

inline UnionDataInformation* OsdParser::unionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    StructOrUnionParsedData supd(info);
    supd.children.reset(new OsdChildrenParser(info, xmlElem.firstChildElement()));
    return DataInformationFactory::newUnion(supd);
}

inline StructureDataInformation* OsdParser::structFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    StructOrUnionParsedData supd(info);
    supd.children.reset(new OsdChildrenParser(info, xmlElem.firstChildElement()));
    return DataInformationFactory::newStruct(supd);
}

EnumDataInformation* OsdParser::enumFromXML(const QDomElement& xmlElem, bool isFlags,
        const OsdParserInfo& info)
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
        const OsdParserInfo& info)
{
    StringParsedData spd(info);
    spd.encoding = readProperty(xmlElem, PROPERTY_ENCODING);
    spd.termination = ParserUtils::uintFromString(readProperty(xmlElem, PROPERTY_TERMINATED_BY));
    spd.maxByteCount = ParserUtils::uintFromString(readProperty(xmlElem, PROPERTY_MAX_BYTE_COUNT));
    spd.maxCharCount = ParserUtils::uintFromString(readProperty(xmlElem, PROPERTY_MAX_CHAR_COUNT));
    return DataInformationFactory::newString(spd);
}

DataInformation* OsdParser::parseElement(const QDomElement& elem, const OsdParserInfo& oldInfo)
{
    Q_ASSERT(!elem.isNull());
    DataInformation* data = 0;
    const QString tag = elem.tagName();
    OsdParserInfo info(oldInfo);
    info.name = readProperty(elem, PROPERTY_NAME, QLatin1String("<anonymous>"));
    if (tag == TYPE_STRUCT)
        data = structFromXML(elem, info);
    else if (tag == TYPE_ARRAY)
        data = arrayFromXML(elem, info);
    else if (tag == TYPE_BITFIELD)
        data = bitfieldFromXML(elem, info);
    else if (tag == TYPE_PRIMITIVE)
        data = primitiveFromXML(elem, info);
    else if (tag == TYPE_UNION)
        data = unionFromXML(elem, info);
    else if (tag == TYPE_ENUM)
        data = enumFromXML(elem, false, info);
    else if (tag == TYPE_FLAGS)
        data = enumFromXML(elem, true, info);
    else if (tag == TYPE_STRING)
        data = stringFromXML(elem, info);
    else if (tag == TYPE_POINTER)
        data = pointerFromXML(elem, info);
    else if (tag == TYPE_TAGGED_UNION)
        data = taggedUnionFromXML(elem, info);
    else
        info.error() << "Unknow tag: " << tag;

    if (data)
    {
        CommonParsedData cpd(info);
        QString byteOrderStr = readProperty(elem, PROPERTY_BYTEORDER);
        if (!byteOrderStr.isEmpty())
            cpd.endianess = ParserUtils::byteOrderFromString(byteOrderStr,
                    LoggerWithContext(info.logger, info.context()));
        cpd.updateFunc = functionSafeEval(info.engine, readProperty(elem, PROPERTY_UPDATE_FUNC));
        cpd.validationFunc = functionSafeEval(info.engine, readProperty(elem, PROPERTY_VALIDATION_FUNC));
        if (!DataInformationFactory::commonInitialization(data, cpd))
        {
            delete data; //error message has already been logged
            return 0;
        }
    }
    return data;
}

EnumDefinition::Ptr OsdParser::findEnum(const QString& defName, const OsdParserInfo& info)
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

QScriptValue OsdParser::functionSafeEval(QScriptEngine* engine, const QString& str)
{
    if (str.isEmpty())
        return QScriptValue();
    //must wrap in parentheses, see https://bugreports.qt-project.org/browse/QTBUG-5757
    QScriptValue ret = engine->evaluate(QLatin1Char('(') + str + QLatin1Char(')'));
    if (!ret.isFunction())
        return QScriptValue(str);
    return ret;
}

TaggedUnionDataInformation* OsdParser::taggedUnionFromXML(const QDomElement& xmlElem,
        const OsdParserInfo& info)
{
    TaggedUnionParsedData tpd(info);
    //can be null
    QDomElement defaultChildren = xmlElem.firstChildElement(PROPERTY_DEFAULT_CHILDREN).firstChildElement();
    if (defaultChildren.isNull())
        info.info() << "No default fields specified, defaulting to none.";
    tpd.defaultFields.reset(new OsdChildrenParser(info, defaultChildren));
    tpd.children.reset(new OsdChildrenParser(info, xmlElem.firstChildElement()));
    //now handle alternatives
    QDomElement alternatives = xmlElem.firstChildElement(PROPERTY_ALTERNATIVES);
    if (alternatives.isNull())
    {
        info.error() << "Missing <alternatives> element, tagged union cannot exist without at least one alternative";
        return 0;
    }
    for (QDomElement elem = alternatives.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        TaggedUnionParsedData::Alternatives alt;
        alt.name = readProperty(elem, PROPERTY_STRUCT_NAME);
        QString selectIfStr = readProperty(elem, PROPERTY_SELECT_IF);
        QScriptValue selectIf = functionSafeEval(info.engine, selectIfStr);
        if (!selectIf.isValid())
            selectIf = selectIfStr;
        alt.selectIf = selectIf;
        if (elem.tagName() == TYPE_GROUP)
            alt.fields = QSharedPointer<ChildrenParser>(new OsdChildrenParser(info, elem.firstChildElement()));
        else
            alt.fields = QSharedPointer<ChildrenParser>(new SingleElementOsdChildrenParser(info, elem));
        tpd.alternatives.append(alt);
    }

    return DataInformationFactory::newTaggedUnion(tpd);
}

QString OsdParser::generateLengthFunction(DataInformation* current, DataInformation* last, QString elemName,
        QString currentString, const ParserInfo& info)
{
    qDebug() << "currentString = " << currentString;
    if (!current) //reached top
        return QString();
    for (int i = current->childCount() - 1; i >= 0; --i)
    {
        DataInformation* child = current->childAt(i);
        if (child == last)
            return QString(); //don't go down again after going up one level

        QString childName = child->name();
        if (childName == elemName)
        {
            QString function = QLatin1String("function() { return this.parent.") + currentString
                    + elemName + QLatin1String(".value; }");
            info.info() << "Found element for dynamic array length: " << child->fullObjectPath()
                    << ", resulting function is: " << function;
            qDebug() << "Found it, currentString = " << currentString << " func is: " << function;
            return function;
        }
        else if (child->childCount() > 0)
        {
            QString func = generateLengthFunction(child, current, elemName,
                    currentString + childName + QLatin1Char('.'), info);
            //if func is empty no valid child was found, just continue
            if (!func.isEmpty())
                return func;
        }
        //has no children and was not the desired element -> continue loop
    }
    //now check parents
    DataInformationBase* nextBase = current->parent();
    if (!nextBase)
        return QString();

    DataInformation* next = nextBase->asDataInformation();
    if (next == last)
        return QString(); //we moved one level down previously, don't move up again
    else
        return generateLengthFunction(current->parent()->asDataInformation(), current, elemName,
                currentString + QLatin1String("parent."), info);
}

OsdChildrenParser::OsdChildrenParser(const OsdParserInfo& info, QDomElement firstChild)
        : mInfo(info), mElem(firstChild)
{
}

DataInformation* OsdChildrenParser::next()
{
    Q_ASSERT(!mElem.isNull());
    //skip all known properties
    while (ALL_PROPERTIES.contains(mElem.tagName()))
        mElem = mElem.nextSiblingElement();
    if (mElem.isNull())
    {
        mInfo.warn() << "Reached end of fields, but next() was requested!";
        return 0;
    }
    DataInformation* ret = OsdParser::parseElement(mElem, mInfo);
    mElem = mElem.nextSiblingElement();
    return ret;
}

OsdChildrenParser::~OsdChildrenParser()
{
}

bool OsdChildrenParser::hasNext()
{
    if (mElem.isNull())
        return false;
    while (ALL_PROPERTIES.contains(mElem.tagName()))
        mElem = mElem.nextSiblingElement(); //skip known properties
    return !mElem.isNull();
}

void OsdChildrenParser::setParent(DataInformation* newParent)
{
    mInfo.parent = newParent;
}

SingleElementOsdChildrenParser::SingleElementOsdChildrenParser(const OsdParserInfo& info, QDomElement element)
    : OsdChildrenParser(info, element), mParsed(false)
{
    if (mElem.isNull())
        info.warn() << "Null Element passed to child parser!";
}

SingleElementOsdChildrenParser::~SingleElementOsdChildrenParser()
{
}

DataInformation* SingleElementOsdChildrenParser::next()
{
    Q_ASSERT(!mParsed);
    mParsed = true;
    return OsdParser::parseElement(mElem, mInfo);
}

bool SingleElementOsdChildrenParser::hasNext()
{
    return !mParsed && !mElem.isNull();
}
