/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OSDPARSER_HPP
#define KASTEN_OSDPARSER_HPP

// lib
#include "abstractstructureparser.hpp"
#include "parserutils.hpp"
#include <enumdefinition.hpp>
// Qt
#include <QDomElement>
// Std
#include <vector>

class QDomDocument;

class StringDataInformation;
class ArrayDataInformation;
class AbstractBitfieldDataInformation;
class EnumDataInformation;
class DataInformation;
class StructureDataInformation;
class UnionDataInformation;
class PrimitiveDataInformation;
class ScriptLogger;

struct OsdParserInfo : public ParserInfo
{
    inline OsdParserInfo(const QString& name, ScriptLogger* logger, DataInformation* parent,
                         QScriptEngine* engine, std::vector<std::shared_ptr<EnumDefinition>>&& enums)
        : ParserInfo(name, logger, parent, engine)
        , enums(std::move(enums))
    {}
    inline OsdParserInfo(const OsdParserInfo& i) = default;
    inline ~OsdParserInfo() = default;

    OsdParserInfo& operator=(const OsdParserInfo&) = delete;

    const std::vector<std::shared_ptr<EnumDefinition>> enums;
};

class OsdParser : public AbstractStructureParser
{
public:
    /** construct a parser which opens parses @p absolutePath */
    OsdParser(const QString& pluginName, const QString& absolutePath);
    /** construct a parser from an in-memory string */
    explicit OsdParser(const QString& xml);
    ~OsdParser() override;

public: // AbstractStructureParser API
    [[nodiscard]]
    QStringList parseStructureNames() const override;
    [[nodiscard]]
    std::vector<std::unique_ptr<TopLevelDataInformation>> parseStructures() const override;

public:
    [[nodiscard]]
    static std::unique_ptr<DataInformation> parseElement(const QDomElement& elem, const OsdParserInfo& oldInfo);

private:
    [[nodiscard]]
    QDomDocument openDoc(ScriptLogger* logger) const;
    [[nodiscard]]
    QDomDocument openDocFromFile(ScriptLogger* logger) const;
    [[nodiscard]]
    QDomDocument openDocFromString(ScriptLogger* logger) const;

private:
    [[nodiscard]]
    static std::unique_ptr<PrimitiveDataInformation> primitiveFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<PointerDataInformation> pointerFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<AbstractBitfieldDataInformation> bitfieldFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<EnumDataInformation> enumFromXML(const QDomElement& elem, bool isFlags, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<StringDataInformation> stringFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<UnionDataInformation> unionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<StructureDataInformation> structFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<ArrayDataInformation> arrayFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    [[nodiscard]]
    static std::unique_ptr<TaggedUnionDataInformation> taggedUnionFromXML(const QDomElement& xmlElem, const OsdParserInfo& info);
    /** Get the child type from the <type> element or type="" attribute.
     * This handles both the case where it is passed as an XML element and as a primitive type string
     * @param xmlElem the parent XML element
     * @param info the parser info
     * @return The parsed element or null if not possible. */
    [[nodiscard]]
    static std::unique_ptr<DataInformation> parseType(const QDomElement& xmlElem, const OsdParserInfo& info, const QString& name);
    [[nodiscard]]
    static std::unique_ptr<DataInformation> parseChildElement(const QDomElement& xmlElem, const OsdParserInfo& info, const QString& name);

    [[nodiscard]]
    static std::shared_ptr<EnumDefinition> findEnum(const QString& defName, const OsdParserInfo& info);

    [[nodiscard]]
    static std::vector<std::shared_ptr<EnumDefinition>> parseEnums(const QDomElement& rootElem, ScriptLogger* logger);

    /** Reads an property of the QDomElement. First it is checked whether an attribute exists, if this is not the case
     * the inner text of an element with tag equal to @p property is returned*/
    [[nodiscard]]
    static QString readProperty(const QDomElement& elem, const QString& property, const QString& defaultVal = QString());

private:
    /** if not empty construct the document from this, instead of opening file */
    const QString mXmlString;
};

class OsdChildrenParser : public ChildrenParser
{
public:
    OsdChildrenParser(const OsdParserInfo& info, const QDomElement& firstChild);
    OsdChildrenParser(const OsdChildrenParser&) = delete;
    OsdChildrenParser(OsdChildrenParser&&) = delete;

    ~OsdChildrenParser() override;

    OsdChildrenParser& operator=(const OsdChildrenParser&) = delete;
    OsdChildrenParser& operator=(OsdChildrenParser&&) = delete;

public: // ChildrenParser API
    [[nodiscard]]
    std::unique_ptr<DataInformation> next() override;
    [[nodiscard]]
    bool hasNext() override;
    void setParent(DataInformation* newParent) override;

protected:
    OsdParserInfo mInfo;
    QDomElement mElem;
};

class SingleElementOsdChildrenParser : public OsdChildrenParser
{
public:
    SingleElementOsdChildrenParser(const OsdParserInfo& info, const QDomElement& element);
    ~SingleElementOsdChildrenParser() override;

public: // ChildrenParser API
    [[nodiscard]]
    std::unique_ptr<DataInformation> next() override;
    [[nodiscard]]
    bool hasNext() override;

private:
    bool mParsed = false;
};

#endif /* KASTEN_OSDPARSER_HPP */
