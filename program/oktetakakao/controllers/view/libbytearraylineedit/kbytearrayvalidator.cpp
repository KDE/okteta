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

#include "kbytearrayvalidator.h"

// Okteta core
#include <valuecodec.h>
#include <khechar.h>
#include <kcharcodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QString>
#include <QtCore/QStringList>


const QStringList &KByteArrayValidator::codingNames()
{
    static QStringList list;
    if( list.isEmpty() )
    {
        list.append( i18nc("Encoding as value","Hexadecimal") );
        list.append( i18nc("Encoding as value","Decimal") );
        list.append( i18nc("Encoding as value","Octal") );
        list.append( i18nc("Encoding as value","Binary") );
        list.append( i18n("Regular Text") );
    }
    return list;
}


KByteArrayValidator::KByteArrayValidator( QObject *parent, int C, int CC )
 : QValidator( parent ), Coding( KHECore::InvalidCoding ),
   ValueCodec( 0 ), CharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) )
{
Q_UNUSED(CC)
    setCoding( C );
}


void KByteArrayValidator::setCharCode( const QString &CodeName )
{
    if( CodeName == CharCodec->name() )
        return;

    delete CharCodec;
    CharCodec = KHECore::KCharCodec::createCodec( CodeName );
}

void KByteArrayValidator::setCoding( int C )
{
    if( C == Coding )
        return;

    Coding = (KHECore::KCoding)C;

    if( Coding != KHECore::CharCoding )
    {
        delete ValueCodec;
        ValueCodec = KHECore::ValueCodec::createCodec( Coding );
    }
}

QValidator::State KByteArrayValidator::validate( QString &String, int &/*pos*/ ) const
{
    State result = QValidator::Acceptable;

    const int StringLength = String.length();
    if( Coding == KHECore::CharCoding )
    {
        for( int i=0; i<StringLength; ++i )
        {
            const QChar Char = String.at( i );
            if( !CharCodec->canEncode(Char) && !Char.isSpace() )
            {
                result = QValidator::Invalid;
                break;
            }
        }
    }
    else
    {
        for( int i=0; i<StringLength; ++i )
        {
            const QChar Char = String.at( i );
            if( !ValueCodec->isValidDigit(Char.toLatin1()) && !Char.isSpace() )
            {
                result = QValidator::Invalid;
                break;
            }
        }
    }

    return result;
}


QByteArray KByteArrayValidator::toByteArray( const QString &String ) const
{
    QByteArray result;

    const int StringSize = String.size();
    if( Coding == KHECore::CharCoding )
    {
        result.resize( StringSize );
        for( int i=0; i<StringSize; ++i )
        {
            char Byte;
            const bool Success = CharCodec->encode( &Byte, String[i] );
            result[i] = Success ? Byte : '?'; // TODO: define unknown symbol
        }
    }
    else
    {
        int i = 0;
        while( i < StringSize )
        {
            unsigned char Byte;
            const int Read = ValueCodec->decode( &Byte, String, i );
            if( Read > 0 )
            {
                i += Read;
                result.append( Byte );
            }
            else
                while( i < StringSize && !ValueCodec->isValidDigit(String[i].toLatin1()) )
                    ++i;
        }
    }
    return result;
}



QString KByteArrayValidator::toString( const QByteArray &ByteArray ) const
{
    QString result;

    const int ByteArraySize = ByteArray.size();
    if( Coding == KHECore::CharCoding )
    {
        result.resize( ByteArraySize );
        for( int i=0; i<ByteArraySize; ++i )
        {
            KHECore::KChar Char = CharCodec->decode( ByteArray[i] );
            result[i] = Char.isUndefined() ? QChar('?') : Char; // TODO: define unknown symbol
        }
    }
    else
    {
        const int EncodingWidth = ValueCodec->encodingWidth();
        result.resize( ByteArraySize * EncodingWidth );
        int r = 0;
        for( int i=0; i<ByteArraySize; ++i,r+=EncodingWidth )
            ValueCodec->encode( result, r, ByteArray[i] );
    }
    return result;
}


KByteArrayValidator::~KByteArrayValidator()
{
    delete ValueCodec;
    delete CharCodec;
}
