/*
    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMDEFINITION_HPP
#define KASTEN_ENUMDEFINITION_HPP

// lib
#include <allprimitivetypes.hpp>
#include <primitivedatatype.hpp>
// Qt
#include <QString>
// Std
#include <map>

class QScriptValue;
struct LoggerWithContext;

struct EnumEntry
{
    AllPrimitiveTypes value;
    QString name;

    [[nodiscard]]
    bool isEmpty() const
    {
        return (value == AllPrimitiveTypes()) && name.isEmpty();
    }
};

class EnumDefinition
{
public:
    EnumDefinition(std::map<AllPrimitiveTypes, QString>&& values, const QString& name,
                   PrimitiveDataType type)
        : mName(name)
        , mValues(std::move(values))
        , mType(type)
    {}

    EnumDefinition(const EnumDefinition& e) = delete;
    EnumDefinition(EnumDefinition&& e) = delete;

    EnumDefinition& operator=(const EnumDefinition& e) = delete;
    EnumDefinition& operator=(EnumDefinition&& e) = delete;

public:
    [[nodiscard]]
    const std::map<AllPrimitiveTypes, QString>& values() const;
    [[nodiscard]]
    AllPrimitiveTypes key(const QString& value) const;
    [[nodiscard]]
    QString value(AllPrimitiveTypes key) const;
    [[nodiscard]]
    PrimitiveDataType type() const;
    [[nodiscard]]
    const QString& name() const;

public:
    [[nodiscard]]
    static std::map<AllPrimitiveTypes, QString> parseEnumValues(const QScriptValue& object,
                                                                const LoggerWithContext& logger, PrimitiveDataType type = PrimitiveDataType::UInt64);
    /** @return a pair containing the converted value. A default constructed pair means error! */
    [[nodiscard]]
    static EnumEntry convertToEnumEntry(const QString& name, const QVariant& value,
                                        const LoggerWithContext& logger, PrimitiveDataType type);

private:
    void setValues(std::map<AllPrimitiveTypes, QString>&& newValues);

private:
    const QString mName;
    std::map<AllPrimitiveTypes, QString> mValues;
    const PrimitiveDataType mType;

    friend class EnumDataInformation;
};

inline const std::map<AllPrimitiveTypes, QString>& EnumDefinition::values() const
{
    return mValues;
}

inline AllPrimitiveTypes EnumDefinition::key(const QString& value) const
{
    const auto it = std::find_if(mValues.begin(), mValues.end(), [&value](const auto& v) {
        return (v.second == value);
    });

    return (it != mValues.end()) ? it->first :  AllPrimitiveTypes();
}

inline QString EnumDefinition::value(AllPrimitiveTypes key) const
{
    const auto it = mValues.find(key);
    return (it != mValues.end()) ? it->second : QString();
}

inline PrimitiveDataType EnumDefinition::type() const
{
    return mType;
}

inline const QString& EnumDefinition::name() const
{
    return mName;
}

inline void EnumDefinition::setValues(std::map<AllPrimitiveTypes, QString>&& newValues)
{
    mValues = newValues;
}

#endif /* KASTEN_ENUMDEFINITION_HPP */
