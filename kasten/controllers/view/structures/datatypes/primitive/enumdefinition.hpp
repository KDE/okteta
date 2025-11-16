/*
    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMDEFINITION_HPP
#define KASTEN_ENUMDEFINITION_HPP

// lib
#include <allprimitivetypes.hpp>
#include <primitivedatatype.hpp>
// Qt core
#include <QMap>
#include <QString>

class QScriptValue;
struct LoggerWithContext;

struct EnumEntry
{
    AllPrimitiveTypes value;
    QString name;

    bool isEmpty() const
    {
        return (value == AllPrimitiveTypes()) && name.isEmpty();
    }
};

class EnumDefinition
{
public:
    EnumDefinition(QMap<AllPrimitiveTypes, QString>&& values, const QString& name,
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
    const QMap<AllPrimitiveTypes, QString>& values() const;
    AllPrimitiveTypes key(const QString& value) const;
    QString value(AllPrimitiveTypes key) const;
    PrimitiveDataType type() const;
    const QString& name() const;

public:
    static QMap<AllPrimitiveTypes, QString> parseEnumValues(const QScriptValue& val,
                                                            const LoggerWithContext& logger, PrimitiveDataType type = PrimitiveDataType::UInt64);
    /** @return a pair containing the converted value. A default constructed pair means error! */
    static EnumEntry convertToEnumEntry(const QString& name, const QVariant& value,
                                        const LoggerWithContext& logger, PrimitiveDataType type);

protected:
    void setValues(QMap<AllPrimitiveTypes, QString>&& newValues);

protected:
    const QString mName;
    QMap<AllPrimitiveTypes, QString> mValues;
    const PrimitiveDataType mType;

    friend class EnumDataInformation;
};

inline const QMap<AllPrimitiveTypes, QString>& EnumDefinition::values() const
{
    return mValues;
}

inline AllPrimitiveTypes EnumDefinition::key(const QString& value) const
{
    return mValues.key(value);
}

inline QString EnumDefinition::value(AllPrimitiveTypes key) const
{
    return mValues.value(key);
}

inline PrimitiveDataType EnumDefinition::type() const
{
    return mType;
}

inline const QString& EnumDefinition::name() const
{
    return mName;
}

inline void EnumDefinition::setValues(QMap<AllPrimitiveTypes, QString>&& newValues)
{
    mValues = newValues;
}

#endif /* KASTEN_ENUMDEFINITION_HPP */
