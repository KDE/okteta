/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVALIDATOR_HPP
#define KASTEN_BYTEARRAYVALIDATOR_HPP

// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QValidator>

namespace Okteta {
class ValueCodec;
class CharCodec;

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
    explicit ByteArrayValidator(QObject* parent = nullptr,
                                Coding codecId = CharCoding,
                                int charCodecId = LocalEncoding);
    ByteArrayValidator() = delete;

    ~ByteArrayValidator() override;

public: // QValidator API
    QValidator::State validate(QString& input, int& pos) const override;

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

public:
    QByteArray toByteArray(const QString& string) const;
    QString toString(const QByteArray& byteArray) const;

private:
    /**
     * Returns a string that is at least as long as @p destLen number of characters,
     * by adding zeroes to the left as necessary.
     *
     * e.g. zeroExtend( "32", 3 ) => "032"
     */
//     QString zeroExtend( const QString &src, int destLen ) const;

    Coding mCodecId = InvalidCoding;
    ValueCodec* mValueCodec = nullptr;
    CharCodec* mCharCodec;
    int mMaxLength = 32767;
    int mMinLength = 0;
};

inline int ByteArrayValidator::maxLength() const { return mMaxLength; }
inline int ByteArrayValidator::minLength() const { return mMinLength; }

}

#endif
