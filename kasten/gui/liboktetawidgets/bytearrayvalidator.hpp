/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVALIDATOR_HPP
#define KASTEN_BYTEARRAYVALIDATOR_HPP

// lib
#include "abstractbytearraystringvalidator.hpp"
// Qt
#include <QValidator>
// Std
#include <memory>

namespace Okteta {

class ByteArrayValidator : public QValidator
{
    Q_OBJECT

public:
    // matching Okteta::ValueCoding
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        CharCoding = 4,
        Utf8Coding = 5
    };

public:
    /// Initial coding: HexadecimalCoding
    explicit ByteArrayValidator(QObject* parent = nullptr);

    ~ByteArrayValidator() override;

public: // QValidator API
    QValidator::State validate(QString& string, int& pos) const override;

public:
    /// Sets one of the value codecs or the current char codec.
    void setCodec(Coding codecId);
    /// Sets the char codec to use. Does not change the current codec.
    void setCharCodec(const QString& charCodecName);
    /// Sets the maximal length of the edited bytearray to @p maxLength.
    /// If @p maxLength is negative, the behaviour is undefined. Default is @c 32767.
    void setMaxLength(int maxLength);
    /// Sets the maximal length of the edited bytearray to @p minLength.
    /// If @p minLength is negative, the behaviour is undefined. Default is @c 0.
    void setMinLength(int minLength);

public:
    int maxLength() const;
    int minLength() const;
    ByteArrayValidator::Coding coding() const;

    QByteArray toByteArray(const QString& string) const;
    QString toString(const QByteArray& byteArray) const;

private:
    std::unique_ptr<AbstractByteArrayStringValidator> m_stringValidator;

    Coding mCodecId = HexadecimalCoding;
    QString m_charCodecName;
};

inline ByteArrayValidator::Coding ByteArrayValidator::coding() const { return mCodecId; }

}

#endif
