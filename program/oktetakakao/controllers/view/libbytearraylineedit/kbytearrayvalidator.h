/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KBYTEARRAYVALIDATOR_H
#define KBYTEARRAYVALIDATOR_H


// Okteta core
#include <khe.h>
// Qt
#include <QtGui/QValidator>

namespace KHECore {
class ValueCodec;
class CharCodec;
}


class KByteArrayValidator : public QValidator
{
  Q_OBJECT

  public:
    // matching KHECore::ValueCoding
    enum Coding { InvalidCoding = -1, HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3, CharCoding=4 };

    static const QStringList &codecNames();
    static const int CodecNumber = 5;

  public:
    explicit KByteArrayValidator( QObject *parent, Coding codecId = CharCoding, int charCodecId = KHECore::LocalEncoding );
    virtual ~KByteArrayValidator();

  public: // QValidator API
    virtual QValidator::State validate( QString &input, int &pos ) const;

  public:
    /// sets one of the value codecs or any char codec
    void setCodec( Coding codecId );
    /// sets the char codec to use
    void setCharCodec( const QString &charCodecName );

  public:
    QByteArray toByteArray( const QString &source ) const;
    QString toString( const QByteArray &source ) const;

  private:
    /**
     * Returns a string that is at least as long as @p destLen number of characters,
     * by adding zeroes to the left as necessary.
     *
     * e.g. zeroExtend( "32", 3 ) => "032"
     */
//     QString zeroExtend( const QString &src, int destLen ) const;

    Coding mCodecId;
    KHECore::ValueCodec *mValueCodec;
    KHECore::CharCodec *mCharCodec;
};

#endif
