/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006,2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BYTEARRAYVALIDATOR_H
#define BYTEARRAYVALIDATOR_H

// Okteta core
#include <oktetacore.h>
// Qt
#include <QtGui/QValidator>


namespace Okteta
{
class ValueCodec;
class CharCodec;


class ByteArrayValidator : public QValidator
{
  Q_OBJECT

  public:
    // matching Okteta::ValueCoding
    enum Coding {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        CharCoding = 4,
        Utf8Coding = 5
    };

  public:
    explicit ByteArrayValidator( QObject* parent = 0,
                                 Coding codecId = CharCoding,
                                 int charCodecId = LocalEncoding );

    virtual ~ByteArrayValidator();

  public: // QValidator API
    virtual QValidator::State validate( QString& input, int& pos ) const;

  public:
    /// Sets one of the value codecs or the current char codec.
    void setCodec( Coding codecId );
    /// Sets the char codec to use. Does not change the current codec.
    void setCharCodec( const QString& charCodecName );
    /// Sets the maximal length of the edited bytearray to @p maxLength.
    /// If @p maxLength is negative, the behaviour is undefined. Default is @c 32767.
    void setMaxLength( int maxLength );
    /// Sets the maximal length of the edited bytearray to @p minLength.
    /// If @p minLength is negative, the behaviour is undefined. Default is @c 0.
    void setMinLength( int minLength );

  public:
    int maxLength() const;
    int minLength() const;

  public:
    QByteArray toByteArray( const QString& string ) const;
    QString toString( const QByteArray& byteArray ) const;

  private:
    /**
     * Returns a string that is at least as long as @p destLen number of characters,
     * by adding zeroes to the left as necessary.
     *
     * e.g. zeroExtend( "32", 3 ) => "032"
     */
//     QString zeroExtend( const QString &src, int destLen ) const;

    Coding mCodecId;
    ValueCodec* mValueCodec;
    CharCodec* mCharCodec;
    int mMaxLength;
    int mMinLength;
};


inline int ByteArrayValidator::maxLength() const { return mMaxLength; }
inline int ByteArrayValidator::minLength() const { return mMinLength; }

}

#endif
