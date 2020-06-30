/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMDEFINITION_HPP
#define KASTEN_ENUMDEFINITION_HPP
#include "../../allprimitivetypes.hpp"
#include "../primitivedatatype.hpp"
// Qt core
#include <QMap>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QPair>

class QScriptValue;
struct LoggerWithContext;

class EnumDefinition : public QSharedData
{
public:
    using Ptr = QSharedDataPointer<EnumDefinition>;

    EnumDefinition(const QMap<AllPrimitiveTypes, QString>& values, const QString& name,
                   PrimitiveDataType type)
        : mName(name)
        , mValues(values)
        , mType(type)
    {}
    EnumDefinition(const EnumDefinition& e)
        : QSharedData(e)
        , mValues(e.mValues)
        , mType(e.mType)
    {}

    const QMap<AllPrimitiveTypes, QString>& values() const;
    const AllPrimitiveTypes key(QString& value) const;
    const QString value(AllPrimitiveTypes key) const;
    PrimitiveDataType type() const;
    const QString& name() const;
    void setValues(const QMap<AllPrimitiveTypes, QString>& newValues);

    static QMap<AllPrimitiveTypes, QString> parseEnumValues(const QScriptValue& val,
                                                            const LoggerWithContext& logger, PrimitiveDataType type = PrimitiveDataType::UInt64);
    /** @return a pair containing the converted value. A default constructed pair means error! */
    static QPair<AllPrimitiveTypes, QString> convertToEnumEntry(const QString& name, const QVariant& value,
                                                                const LoggerWithContext& logger, PrimitiveDataType type);

protected:
    const QString mName;
    QMap<AllPrimitiveTypes, QString> mValues;
    const PrimitiveDataType mType;
};

inline const QMap<AllPrimitiveTypes, QString>& EnumDefinition::values() const
{
    return mValues;
}

inline const AllPrimitiveTypes EnumDefinition::key(QString& value) const
{
    return mValues.key(value);
}

inline const QString EnumDefinition::value(AllPrimitiveTypes key) const
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

inline void EnumDefinition::setValues(const QMap<AllPrimitiveTypes, QString>& newValues)
{
    mValues = newValues; // causes the QSharedPointer to detach and copy
}

#endif /* KASTEN_ENUMDEFINITION_HPP */
