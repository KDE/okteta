/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "datatypedisplaystringifiermanager.hpp"

// lib
#include "abstractdatatypedisplaystringifier.hpp"
// libdatatypes
#include <binary8.hpp>
#include <octal8.hpp>
#include <hexadecimal8.hpp>
#include <float32.hpp>
#include <float64.hpp>
#include <char8.hpp>
#include <utf8.hpp>
#include <utf16.hpp>
#include <sint8.hpp>
#include <sint16.hpp>
#include <sint32.hpp>
#include <sint64.hpp>
#include <uint8.hpp>
#include <uint16.hpp>
#include <uint32.hpp>
#include <uint64.hpp>
// Qt
#include <QLocale>

namespace Okteta {

class Binary8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(locale)
        Q_UNUSED(asHex)

        const Okteta::Binary8 binary8 = data.value<Okteta::Binary8>();
        return binary8.toString();
    }
};

class Octal8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(locale)
        Q_UNUSED(asHex)

        const Okteta::Octal8 octal8 = data.value<Okteta::Octal8>();
        return octal8.toString();
    }
};

class Hexadecimal8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(locale)
        Q_UNUSED(asHex)

        const Okteta::Hexadecimal8 hexadecimal8 = data.value<Okteta::Hexadecimal8>();
        return hexadecimal8.toString();
    }
};

class SInt8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(asHex)

        const Okteta::SInt8 sInt8 = data.value<Okteta::SInt8>();
        return sInt8.toString(locale);
    }
};

class SInt16DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(asHex)

        const Okteta::SInt16 sInt16 = data.value<Okteta::SInt16>();
        return sInt16.toString(locale);
    }
};

class SInt32DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(asHex)

        const Okteta::SInt32 sInt32 = data.value<Okteta::SInt32>();
        return sInt32.toString(locale);
    }
};

class SInt64DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(asHex)

        const Okteta::SInt64 sInt64 = data.value<Okteta::SInt64>();
        return sInt64.toString(locale);
    }
};

class UInt8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        const Okteta::UInt8 uInt8 = data.value<Okteta::UInt8>();
        return uInt8.toString(asHex, locale);
    }
};

class UInt16DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        const Okteta::UInt16 uInt16 = data.value<Okteta::UInt16>();
        return uInt16.toString(asHex, locale);
    }
};

class UInt32DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        const Okteta::UInt32 uInt32 = data.value<Okteta::UInt32>();
        return uInt32.toString(asHex, locale);
    }
};

class UInt64DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        const Okteta::UInt64 uInt64 = data.value<Okteta::UInt64>();
        return uInt64.toString(asHex, locale);
    }
};

class Float32DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(asHex)

        const Okteta::Float32 float32 = data.value<Okteta::Float32>();
        return float32.toString(locale);
    }
};

class Float64DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(asHex)

        const Okteta::Float64 float64 = data.value<Okteta::Float64>();
        return float64.toString(locale);
    }
};

class Char8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(locale)
        Q_UNUSED(asHex)

        const Okteta::Char8 char8 = data.value<Okteta::Char8>();
        return char8.toString();
    }
};

class Utf8DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(locale)
        Q_UNUSED(asHex)

        const Okteta::Utf8 utf8 = data.value<Okteta::Utf8>();
        return utf8.toString();
    }
};

class Utf16DisplayStringifier : public AbstractDataTypeDisplayStringifier
{
public:
    QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const override
    {
        Q_UNUSED(locale)
        Q_UNUSED(asHex)

        const Okteta::Utf16 utf16 = data.value<Okteta::Utf16>();
        return utf16.toString();
    }
};

DataTypeDisplayStringifierManager::DataTypeDisplayStringifierManager()
{
    m_stringifiers = {
        {qMetaTypeId<Okteta::Binary8>(),      new Binary8DisplayStringifier},
        {qMetaTypeId<Okteta::Octal8>(),       new Octal8DisplayStringifier},
        {qMetaTypeId<Okteta::Hexadecimal8>(), new Hexadecimal8DisplayStringifier},
        {qMetaTypeId<Okteta::SInt8>(),        new SInt8DisplayStringifier},
        {qMetaTypeId<Okteta::SInt16>(),       new SInt16DisplayStringifier},
        {qMetaTypeId<Okteta::SInt32>(),       new SInt32DisplayStringifier},
        {qMetaTypeId<Okteta::SInt64>(),       new SInt64DisplayStringifier},
        {qMetaTypeId<Okteta::UInt8>(),        new UInt8DisplayStringifier},
        {qMetaTypeId<Okteta::UInt16>(),       new UInt16DisplayStringifier},
        {qMetaTypeId<Okteta::UInt32>(),       new UInt32DisplayStringifier},
        {qMetaTypeId<Okteta::UInt64>(),       new UInt64DisplayStringifier},
        {qMetaTypeId<Okteta::Float32>(),      new Float32DisplayStringifier},
        {qMetaTypeId<Okteta::Float64>(),      new Float64DisplayStringifier},
        {qMetaTypeId<Okteta::Char8>(),        new Char8DisplayStringifier},
        {qMetaTypeId<Okteta::Utf8>(),         new Utf8DisplayStringifier},
        {qMetaTypeId<Okteta::Utf16>(),        new Utf16DisplayStringifier},
    };
}

DataTypeDisplayStringifierManager::~DataTypeDisplayStringifierManager()
{
    qDeleteAll(m_stringifiers);
}

AbstractDataTypeDisplayStringifier* DataTypeDisplayStringifierManager::stringifier(int metaTypeId) const
{
    return m_stringifiers.value(metaTypeId, nullptr);
}

}
