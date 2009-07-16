/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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
#include <character.h>
#include <charcodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QString>
#include <QtCore/QStringList>


const QStringList &KByteArrayValidator::codecNames()
{
    static QStringList list;
    if( list.isEmpty() )
    {
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the hexadecimal format","Hexadecimal")  );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the decimal format",    "Decimal")      );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the octal format",      "Octal")        );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the binary format",     "Binary")       );
        list.append( i18nc("@item:inlistbox coding of the bytes as characters with the values",      "Character(s)") );
    }
    return list;
}


KByteArrayValidator::KByteArrayValidator( QObject *parent, Coding codecId, int charCodecId )
 : QValidator( parent ), mCodecId( InvalidCoding ),
   mValueCodec( 0 ), mCharCodec( Okteta::CharCodec::createCodec(Okteta::LocalEncoding) )
{
Q_UNUSED(charCodecId)
    setCodec( codecId );
}


void KByteArrayValidator::setCharCodec( const QString &charCodecName )
{
    if( charCodecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = Okteta::CharCodec::createCodec( charCodecName );
}

void KByteArrayValidator::setCodec( Coding codecId )
{
    if( codecId == mCodecId )
        return;

    mCodecId = codecId;

    if( mCodecId != CharCoding )
    {
        delete mValueCodec;
        mValueCodec = Okteta::ValueCodec::createCodec( (Okteta::ValueCoding)mCodecId );
    }
}

QValidator::State KByteArrayValidator::validate( QString &string, int &/*pos*/ ) const
{
    State result = QValidator::Acceptable;

    const int stringLength = string.length();
    if( mCodecId == CharCoding )
    {
        for( int i=0; i<stringLength; ++i )
        {
            const QChar c = string.at( i );
            if( !mCharCodec->canEncode(c) && !c.isSpace() )
            {
                result = QValidator::Invalid;
                break;
            }
        }
    }
    else
    {
        for( int i=0; i<stringLength; ++i )
        {
            const QChar c = string.at( i );
            if( !mValueCodec->isValidDigit(c.toLatin1()) && !c.isSpace() )
            {
                result = QValidator::Invalid;
                break;
            }
        }
    }

    return result;
}


QByteArray KByteArrayValidator::toByteArray( const QString &string ) const
{
    QByteArray result;

    const int stringLength = string.length();
    if( mCodecId == CharCoding )
    {
        result.resize( stringLength );
        for( int i=0; i<stringLength; ++i )
        {
            char byte;
            const bool success = mCharCodec->encode( &byte, string[i] );
            result[i] = success ? byte : '?'; // TODO: define unknown symbol
        }
    }
    else
    {
        int i = 0;
        while( i < stringLength )
        {
            unsigned char byte;
            const int readChars = mValueCodec->decode( &byte, string, i );
            if( readChars > 0 )
            {
                i += readChars;
                result.append( byte );
            }
            else
                while( i < stringLength && !mValueCodec->isValidDigit(string[i].toLatin1()) )
                    ++i;
        }
    }
    return result;
}



QString KByteArrayValidator::toString( const QByteArray &byteArray ) const
{
    QString result;

    const int byteArraySize = byteArray.size();
    if( mCodecId == CharCoding )
    {
        result.resize( byteArraySize );
        for( int i=0; i<byteArraySize; ++i )
        {
            Okteta::Character c = mCharCodec->decode( byteArray[i] );
            result[i] = c.isUndefined() ? QChar('?') : c; // TODO: define unknown symbol
        }
    }
    else
    {
        const int encodingWidth = mValueCodec->encodingWidth();
        result.resize( byteArraySize * encodingWidth );
        int r = 0;
        for( int i=0; i<byteArraySize; ++i,r+=encodingWidth )
            mValueCodec->encode( result, r, byteArray[i] );
    }
    return result;
}


KByteArrayValidator::~KByteArrayValidator()
{
    delete mValueCodec;
    delete mCharCodec;
}
