/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_CORE_KTEXTCHARCODEC_H
#define KHE_CORE_KTEXTCHARCODEC_H

// lib
#include <kcharcodec.h>
// Qt
#include <QtCore/QString>

class QTextCodec;
class QTextDecoder;
class QTextEncoder;


namespace KHECore
{

// used by all codecs with full char coping, i.e. there are no undefined chars
class KTextCharCodec : public KCharCodec
{
  public:
    static KTextCharCodec *createCodec( const QString &CodeName );
    static KTextCharCodec *createCodec( KEncoding C );
    static KTextCharCodec *createLocalCodec();

    static const QStringList &codecNames();

  protected:
    KTextCharCodec( QTextCodec *C );
  public:
    virtual ~KTextCharCodec();

  public: // KCharCodec API
    virtual bool encode( char *D, const QChar &C ) const;
    virtual KChar decode( char Byte ) const;
    virtual bool canEncode( const QChar &C ) const;
    virtual const QString& name() const;

  protected:
    QTextCodec *Codec;
    /** decodes the chars to unicode */
    QTextDecoder *Decoder;
    /** encodes the chars from unicode */
    QTextEncoder *Encoder;
    /** */
    mutable QString Name;
};

}

#endif
