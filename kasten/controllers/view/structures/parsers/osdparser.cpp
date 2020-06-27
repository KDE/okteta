/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "osdparser.hpp"

#include "datainformationfactory.hpp"
#include "../datatypes/array/arraydatainformation.hpp"
#include "../datatypes/uniondatainformation.hpp"
#include "../datatypes/structuredatainformation.hpp"
#include "../datatypes/strings/stringdatainformation.hpp"
#include "../datatypes/strings/stringdata.hpp"
#include "../datatypes/primitivefactory.hpp"
#include "../datatypes/topleveldatainformation.hpp"
#include "../datatypes/dummydatainformation.hpp"
#include "../structuredefinitionfile.hpp"
#include "../script/scriptlogger.hpp"
#include "../script/scriptengineinitializer.hpp"
#include "../structlogging.hpp"

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
    : AbstractStructureParser(QString(), QString())
    , mXmlString(xml)
{
}

OsdParser::~OsdParser() = default;

QDomDocument OsdParser::openDoc(ScriptLogger* logger) const
{
    return mXmlString.isEmpty() ? openDocFromFile(logger) : openDocFromString(logger);
}

QDomDocument OsdParser::openDocFromString(ScriptLogger* logger) const
{
    Q_CHECK_PTR(logger);
    Q_ASSERT(!mXmlString.isEmpty());
    int errorLine, errorColumn;
    QString errorMsg;
    QDomDocument doc;
    if (!doc.setContent(mXmlString, false, &errorMsg, &errorLine, &errorColumn)) {
        const QString errorOutput =
            QStringLiteral("error reading XML: %1\n error line=%2\nerror column=%3")
            .arg(errorMsg, QString::number(errorLine), QString::number(errorColumn));
        logger->error() << errorOutput;
        logger->info() << "XML was:" << mXmlString;

        return {};
    }
    return doc;
}

QDomDocument OsdParser::openDocFromFile(ScriptLogger* logger) const
{
    Q_CHECK_PTR(logger);
    QFileInfo fileInfo(mAbsolutePath);
    if (!fileInfo.exists()) {
        logger->error() << "File" << mAbsolutePath << "does not exist!";
        return {};
    }
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        const QString errorOutput = QLatin1String("Could not open file ") + mAbsolutePath;
        logger->error() << errorOutput;
        return {};
    }
    int errorLine, errorColumn;
    QString errorMsg;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn)) {
        const QString errorOutput =
            QStringLiteral("error reading XML: %1\n error line=%2\nerror column=%3")
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
    QScopedPointer<ScriptLogger> rootLogger(new ScriptLogger); // only needed if we get an error right now
    rootLogger->setLogToStdOut(true); // we cannot get our messages into the script console, so do this instead
    QDomDocument document = openDoc(rootLogger.data());
    if (document.isNull()) {
        return {};
    }
    QDomElement rootElem = document.firstChildElement(QStringLiteral("data"));
    if (rootElem.isNull()) {
        return {};
    }
    for (QDomElement childElement = rootElem.firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        QString tag = childElement.tagName();
        if (tag == TYPE_STRUCT() || tag == TYPE_ARRAY() || tag == TYPE_BITFIELD() || tag == TYPE_PRIMITIVE()
            || tag == TYPE_UNION() || tag == TYPE_ENUM() || tag == TYPE_FLAGS() || tag == TYPE_STRING()) {
            // TODO allow e.g. <uint8 name="asfd">
            ret.append(readProperty(childElement, PROPERTY_NAME(), i18n("<invalid name>")));
        } else {
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
    QScopedPointer<ScriptLogger> rootLogger(new ScriptLogger()); // only needed in we get an error right now
    QDomDocument document = openDoc(rootLogger.data());
    if (document.isNull()) {
        structures.append(new TopLevelDataInformation(
                              new DummyDataInformation(nullptr, fileInfo.fileName()), rootLogger.take(), nullptr, fileInfo));
        return structures;
    }

    QDomElement rootElem = document.firstChildElement(QStringLiteral("data"));
    if (rootElem.isNull()) {
        rootLogger->error() << "Missing top level <data> element!";
        structures.append(new TopLevelDataInformation(
                              new DummyDataInformation(nullptr, fileInfo.fileName()), rootLogger.take(), nullptr, fileInfo));
        return structures;
    }
    int count = 1;
    for (QDomElement elem = rootElem.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement()) {
        if (elem.tagName() == TYPE_ENUMDEF()) {
            continue; // skip enum defs

        }
        QScriptEngine* eng = ScriptEngineInitializer::newEngine(); // we need this for dynamic arrays
        auto* logger = new ScriptLogger();
        QVector<EnumDefinition::Ptr> enums = parseEnums(rootElem, logger);
        OsdParserInfo info(QString(), logger, nullptr, eng, enums);

        DataInformation* data = parseElement(elem, info);

        if (!data) {
            QString name = readProperty(elem, PROPERTY_NAME());
            if (name.isEmpty()) {
                name = fileInfo.absoluteFilePath() + QLatin1String("_element") + QString::number(count);
            }
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Failed to parse element" << elem.tagName() << name;
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Parsing messages were:" << logger->messages();
            data = new DummyDataInformation(nullptr, name);
        }
        auto* topData = new TopLevelDataInformation(data, logger, eng, fileInfo);
        QString lockOffsetStr = readProperty(elem, PROPERTY_DEFAULT_LOCK_OFFSET());
        if (!lockOffsetStr.isEmpty()) {
            ParsedNumber<quint64> offset = ParserUtils::uint64FromString(lockOffsetStr);
            if (!offset.isValid) {
                data->logError() << "Default lock offset is not a valid number:" << offset.string;
            } else {
                data->logInfo() << "Default lock offset is " << offset.string;
                topData->setDefaultLockOffset(offset.value);
            }
        }
        structures.append(topData);
        count++;
    }

    return structures;
}

// TODO make type depend on the user not the definition
QVector<EnumDefinition::Ptr> OsdParser::parseEnums(const QDomElement& rootElem, ScriptLogger* logger)
{
    QVector<EnumDefinition::Ptr> ret;
    for (QDomElement elem = rootElem.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement()) {
        if (elem.tagName() != TYPE_ENUMDEF()) {
            continue;
        }

        QMap<AllPrimitiveTypes, QString> defs;
        const QString enumName = readProperty(elem, PROPERTY_NAME(), i18n("<invalid name>"));
        const QString typeStr = readProperty(elem, PROPERTY_TYPE());
        if (typeStr.isEmpty()) {
            logger->error(enumName) << "Skipping enum definition, since no type attribute was found.";
            continue;
        }
        LoggerWithContext lwc(logger, QLatin1String("enum values (") + enumName + QLatin1Char(')'));
        PrimitiveDataType type = PrimitiveFactory::typeStringToType(typeStr, lwc);
        // handle all entries
        for (QDomElement child = elem.firstChildElement(); !child.isNull(); child =
                 child.nextSiblingElement()) {
            if (child.tagName() != QLatin1String("entry")) {
                continue;
            }

            QString name = readProperty(child, PROPERTY_NAME());
            if (name.isEmpty()) {
                lwc.warn() << "Entry is missing name, skipping it!";
                continue;
            }
            QString value = readProperty(child, PROPERTY_VALUE());
            QPair<AllPrimitiveTypes, QString> converted =
                EnumDefinition::convertToEnumEntry(name, value, lwc, type);
            if (converted == QPair<AllPrimitiveTypes, QString>()) {
                continue;
            }
            defs.insert(converted.first, converted.second);
        }

        // now add this enum to the list of enums
        if (defs.isEmpty()) {
            lwc.error() << "Enum definition contains no valid elements!";
        } else {
            EnumDefinition::Ptr enumDef = EnumDefinition::Ptr(new EnumDefinition(defs, enumName, type));
            ret.append(enumDef);
        }
    }

    return ret;
}

// Datatypes

ArrayDataInformation* OsdParser::arrayFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    ArrayParsedData apd(info);
    QString lengthStr = readProperty(xmlElem, PROPERTY_LENGTH());
    if (lengthStr.isEmpty()) {
        info.error() << "No array length specified!";
        return nullptr;
    }
    // must wrap in parentheses, cannot just evaluate. See https://bugreports.qt-project.org/browse/QTBUG-5757
    const QScriptValue lengthFunc = ParserUtils::functionSafeEval(info.engine, lengthStr);
    if (lengthFunc.isValid()) {
        apd.length = lengthFunc;
    } else {
        apd.length = QScriptValue(lengthStr);
    }
    // first check whether there is a <type> element and use the inner element
    // if that doesn't exist use the first child element as the type, but only if there is only one child
    apd.arrayType = parseType(xmlElem, info, NAME_ARRAY_TYPE());
    if (!apd.arrayType) {
        // was not specified as <type> element or type="attribute", use first child
        DummyDataInformation dummy(info.parent, info.name); // dummy so that we have a proper chain
        OsdChildrenParser typeParser(info, xmlElem.firstChildElement());
        typeParser.setParent(&dummy);
        if (typeParser.hasNext()) {
            apd.arrayType = typeParser.next();
            if (typeParser.hasNext()) {
                info.error() << "More than one possible type for array!";
                delete apd.arrayType;
                apd.arrayType = nullptr;
                return nullptr;
            }
        }
    }
    return DataInformationFactory::newArray(apd);
}

DataInformation* OsdParser::parseChildElement(const QDomElement& xmlElem, const OsdParserInfo& info, const QString& name)
{
    OsdParserInfo newInfo(info);
    // instantiate a dummy so that a property chain up to the root element exists
    DummyDataInformation dummy(info.parent, info.name);
    newInfo.parent = &dummy;
    newInfo.name = name;
    return parseElement(xmlElem, newInfo);
}

DataInformation* OsdParser::parseType(const QDomElement& xmlElem, const OsdParserInfo& info, const QString& name)
{
    const QString typeAttribute = xmlElem.attribute(PROPERTY_TYPE());
    if (!typeAttribute.isEmpty()) {
        // type was specified as a primitive string
        LoggerWithContext lwc(info.logger, info.context() + name);
        DataInformation* ret = PrimitiveFactory::newInstance(name, typeAttribute, lwc);
        if (!ret) {
            info.error() << typeAttribute << "is not a valid type identifier";
        }
        return ret;
    }
    // we have to parse the first child element of the <type> element
    const QDomElement toParse = xmlElem.firstChildElement(PROPERTY_TYPE()).firstChildElement();
    if (toParse.isNull()) {
        // don't log an error here, it may be okay (i.e. in arrays <type> can be omitted)
        return nullptr;
    }
    if (!toParse.nextSiblingElement().isNull()) {
        info.warn() << "<type> element has more than one child!";
    }
    // TODO have this newInfo code only in one location
    DataInformation* ret = parseChildElement(toParse, info, name);
    if (!ret) {
        info.error() << "Failed to parse element defined in <type>";
    }
    return ret;
}

PointerDataInformation* OsdParser::pointerFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    PointerParsedData ppd(info);

    ppd.valueType = parseType(xmlElem, info, NAME_POINTER_VALUE_TYPE());

    // first check whether there is a <target> element and use the inner element
    // if that doesn't exist use the first child element as the type, but only if there is only one child
    QDomElement childElement = xmlElem.firstChildElement(PROPERTY_TARGET()).firstChildElement();
    if (childElement.isNull()) {
        childElement = xmlElem.firstChildElement();
        if (childElement.isNull()) {
            info.error() << "Pointer target is missing! Please add a <target> child element.";
            return nullptr;
        }
        if (childElement != xmlElem.lastChildElement()) {
            // there is more than one child element
            info.error() << "There is more than one child element, cannot determine which one "
                "is the pointer target. Wrap the correct one in a <target> element.";
            return nullptr;
        }
    }
    ppd.pointerTarget = parseChildElement(childElement, info, NAME_POINTER_TARGET());
    return DataInformationFactory::newPointer(ppd);
}

PrimitiveDataInformation* OsdParser::primitiveFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    PrimitiveParsedData ppd(info);
    ppd.type = readProperty(xmlElem, PROPERTY_TYPE());
    return DataInformationFactory::newPrimitive(ppd);
}

AbstractBitfieldDataInformation* OsdParser::bitfieldFromXML(const QDomElement& xmlElem,
                                                            const OsdParserInfo& info)
{
    BitfieldParsedData bpd(info);
    bpd.type = readProperty(xmlElem, PROPERTY_TYPE());
    QString width = readProperty(xmlElem, PROPERTY_WIDTH());
    bpd.width = ParserUtils::intFromString(width);
    return DataInformationFactory::newBitfield(bpd);
}

UnionDataInformation* OsdParser::unionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    StructOrUnionParsedData supd(info);
    supd.children.reset(new OsdChildrenParser(info, xmlElem.firstChildElement()));
    return DataInformationFactory::newUnion(supd);
}

StructureDataInformation* OsdParser::structFromXML(const QDomElement& xmlElem, const OsdParserInfo& info)
{
    StructOrUnionParsedData supd(info);
    supd.children.reset(new OsdChildrenParser(info, xmlElem.firstChildElement()));
    return DataInformationFactory::newStruct(supd);
}

EnumDataInformation* OsdParser::enumFromXML(const QDomElement& xmlElem, bool isFlags,
                                            const OsdParserInfo& info)
{
    EnumParsedData epd(info);
    epd.type = readProperty(xmlElem, PROPERTY_TYPE());
    epd.enumName = readProperty(xmlElem, PROPERTY_ENUM_NAME());
    if (epd.enumName.isEmpty()) {
        epd.enumName = readProperty(xmlElem, TYPE_ENUM()); // used again here as property
    }
    epd.enumDef = findEnum(epd.enumName, info);
    if (!epd.enumDef) {
        info.error().nospace() << "Enum definition '" << epd.enumName << "' does not exist!";
        return nullptr;
    }

    if (isFlags) {
        return DataInformationFactory::newFlags(epd);
    }

    return DataInformationFactory::newEnum(epd);
}

StringDataInformation* OsdParser::stringFromXML(const QDomElement& xmlElem,
                                                const OsdParserInfo& info)
{
    StringParsedData spd(info);
    spd.encoding = readProperty(xmlElem, PROPERTY_ENCODING());
    spd.termination = ParserUtils::uintFromString(readProperty(xmlElem, PROPERTY_TERMINATED_BY()));
    spd.maxByteCount = ParserUtils::uintFromString(readProperty(xmlElem, PROPERTY_MAX_BYTE_COUNT()));
    spd.maxCharCount = ParserUtils::uintFromString(readProperty(xmlElem, PROPERTY_MAX_CHAR_COUNT()));
    return DataInformationFactory::newString(spd);
}

DataInformation* OsdParser::parseElement(const QDomElement& elem, const OsdParserInfo& oldInfo)
{
    Q_ASSERT(!elem.isNull());
    DataInformation* data = nullptr;
    const QString tag = elem.tagName();
    OsdParserInfo info(oldInfo);
    info.name = readProperty(elem, PROPERTY_NAME(), QStringLiteral("<anonymous>"));
    if (tag == TYPE_STRUCT()) {
        data = structFromXML(elem, info);
    } else if (tag == TYPE_ARRAY()) {
        data = arrayFromXML(elem, info);
    } else if (tag == TYPE_BITFIELD()) {
        data = bitfieldFromXML(elem, info);
    } else if (tag == TYPE_PRIMITIVE()) {
        data = primitiveFromXML(elem, info);
    } else if (tag == TYPE_UNION()) {
        data = unionFromXML(elem, info);
    } else if (tag == TYPE_ENUM()) {
        data = enumFromXML(elem, false, info);
    } else if (tag == TYPE_FLAGS()) {
        data = enumFromXML(elem, true, info);
    } else if (tag == TYPE_STRING()) {
        data = stringFromXML(elem, info);
    } else if (tag == TYPE_POINTER()) {
        data = pointerFromXML(elem, info);
    } else if (tag == TYPE_TAGGED_UNION()) {
        data = taggedUnionFromXML(elem, info);
    } else {
        LoggerWithContext lwc(info.logger, info.context());
        // use the type tag as a primitive type
        data = PrimitiveFactory::newInstance(info.name, tag, lwc);
        if (!data) {
            info.error() << "Cannot parse unknown tag: " << tag;
        }
    }

    if (data) {
        CommonParsedData cpd(info);
        QString byteOrderStr = readProperty(elem, PROPERTY_BYTEORDER());
        if (!byteOrderStr.isEmpty()) {
            cpd.endianess = ParserUtils::byteOrderFromString(byteOrderStr,
                                                             LoggerWithContext(info.logger, info.context()));
        }
        cpd.updateFunc = ParserUtils::functionSafeEval(info.engine, readProperty(elem, PROPERTY_UPDATE_FUNC()));
        cpd.validationFunc = ParserUtils::functionSafeEval(info.engine, readProperty(elem, PROPERTY_VALIDATION_FUNC()));
        cpd.toStringFunc = ParserUtils::functionSafeEval(info.engine, readProperty(elem, PROPERTY_TO_STRING_FUNC()));
        cpd.customTypeName = readProperty(elem, PROPERTY_CUSTOM_TYPE_NAME());
        if (!DataInformationFactory::commonInitialization(data, cpd)) {
            delete data; // error message has already been logged
            return nullptr;
        }
    }
    return data;
}

EnumDefinition::Ptr OsdParser::findEnum(const QString& defName, const OsdParserInfo& info)
{
    for (const auto& def : info.enums) {
        if (def->name() == defName) {
            return def;
        }
    }

    info.error() << "Could not find enum definition with name" << defName;
    return EnumDefinition::Ptr(nullptr);
}

QString OsdParser::readProperty(const QDomElement& elem, const QString& property, const QString& defaultVal)
{
    const QString attrib = elem.attribute(property);
    if (!attrib.isEmpty()) {
        return attrib;
    }
    // check for element now
    const QDomElement childElem = elem.firstChildElement(property);
    if (!elem.isNull()) {
        return elem.text();
    }

    return defaultVal;
}

TaggedUnionDataInformation* OsdParser::taggedUnionFromXML(const QDomElement& xmlElem,
                                                          const OsdParserInfo& info)
{
    TaggedUnionParsedData tpd(info);
    // can be null
    QDomElement defaultChildren = xmlElem.firstChildElement(PROPERTY_DEFAULT_CHILDREN()).firstChildElement();
    if (defaultChildren.isNull()) {
        info.info() << "No default fields specified, defaulting to none.";
    }
    tpd.defaultFields.reset(new OsdChildrenParser(info, defaultChildren));
    tpd.children.reset(new OsdChildrenParser(info, xmlElem.firstChildElement()));
    // now handle alternatives
    QDomElement alternatives = xmlElem.firstChildElement(PROPERTY_ALTERNATIVES());
    if (alternatives.isNull()) {
        info.error() << "Missing <alternatives> element, tagged union cannot exist without at least one alternative";
        return nullptr;
    }
    for (QDomElement elem = alternatives.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement()) {
        TaggedUnionParsedData::Alternatives alt;
        alt.name = readProperty(elem, PROPERTY_STRUCT_NAME());
        QString selectIfStr = readProperty(elem, PROPERTY_SELECT_IF());
        QScriptValue selectIf = ParserUtils::functionSafeEval(info.engine, selectIfStr);
        if (!selectIf.isValid()) {
            selectIf = selectIfStr;
        }
        alt.selectIf = selectIf;
        if (elem.tagName() == TYPE_GROUP()) {
            alt.fields = QSharedPointer<ChildrenParser>(new OsdChildrenParser(info, elem.firstChildElement()));
        } else {
            alt.fields = QSharedPointer<ChildrenParser>(new SingleElementOsdChildrenParser(info, elem));
        }
        tpd.alternatives.append(alt);
    }

    return DataInformationFactory::newTaggedUnion(tpd);
}

OsdChildrenParser::OsdChildrenParser(const OsdParserInfo& info, const QDomElement& firstChild)
    : mInfo(info)
    , mElem(firstChild)
{
}

OsdChildrenParser::~OsdChildrenParser() = default;

DataInformation* OsdChildrenParser::next()
{
    Q_ASSERT(!mElem.isNull());
    // skip all known properties
    const QStringList allProperties = ALL_PROPERTIES();
    while (allProperties.contains(mElem.tagName())) {
        mElem = mElem.nextSiblingElement();
    }
    if (mElem.isNull()) {
        mInfo.warn() << "Reached end of fields, but next() was requested!";
        return nullptr;
    }
    DataInformation* ret = OsdParser::parseElement(mElem, mInfo);
    mElem = mElem.nextSiblingElement();
    return ret;
}

bool OsdChildrenParser::hasNext()
{
    if (mElem.isNull()) {
        return false;
    }
    const QStringList allProperties = ALL_PROPERTIES();
    while (allProperties.contains(mElem.tagName())) {
        mElem = mElem.nextSiblingElement(); // skip known properties
    }
    return !mElem.isNull();
}

void OsdChildrenParser::setParent(DataInformation* newParent)
{
    mInfo.parent = newParent;
}

SingleElementOsdChildrenParser::SingleElementOsdChildrenParser(const OsdParserInfo& info, const QDomElement& element)
    : OsdChildrenParser(info, element)
    , mParsed(false)
{
    if (mElem.isNull()) {
        info.warn() << "Null Element passed to child parser!";
    }
}

SingleElementOsdChildrenParser::~SingleElementOsdChildrenParser() = default;

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
