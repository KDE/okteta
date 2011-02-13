/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2004,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_TEXTCHARCODEC_H
#define OKTETA_TEXTCHARCODEC_H

// lib
#include <charcodec.h>
// Qt
#include <QtCore/QString>

class QTextCodec;
class QTextDecoder;
class QTextEncoder;


namespace Okteta
{

// used by all codecs with full char coping, i.e. there are no undefined chars
class TextCharCodec : public CharCodec
{
  public:
    static TextCharCodec* createCodec( const QString& codeName );
    static TextCharCodec* createCodec( CharCoding charCoding );
    static TextCharCodec* createLocalCodec();

    static const QStringList& codecNames();

  protected:
    TextCharCodec( QTextCodec* textCodec );
  public:
    virtual ~TextCharCodec();

  public: // CharCodec API
    virtual Character decode( Byte byte ) const;
    virtual bool encode( Byte* byte, const QChar& _char ) const;
    virtual bool canEncode( const QChar& _char ) const;
    virtual const QString& name() const;

  protected:
    QTextCodec* mCodec;
    /** decodes the chars to unicode */
    QTextDecoder* mDecoder;
    /** encodes the chars from unicode */
    QTextEncoder* mEncoder;
    /** */
    mutable QString mName;
};

}

#endif
