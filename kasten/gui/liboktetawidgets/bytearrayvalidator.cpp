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

#include "bytearrayvalidator.h"

// Okteta core
#include <valuecodec.h>
#include <character.h>
#include <charcodec.h>


namespace Okteta
{

ByteArrayValidator::ByteArrayValidator( QObject* parent, Coding codecId, int charCodecId )
  : QValidator( parent ),
    mCodecId( InvalidCoding ),
    mValueCodec( 0 ),
    mCharCodec( CharCodec::createCodec(Okteta::LocalEncoding) ),
    mMaxLength( 32767 ),
    mMinLength( 0 )
{
Q_UNUSED(charCodecId)
    setCodec( codecId );
}


void ByteArrayValidator::setCharCodec( const QString& charCodecName )
{
    if( charCodecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = CharCodec::createCodec( charCodecName );
}

void ByteArrayValidator::setCodec( Coding codecId )
{
    if( codecId == mCodecId )
        return;

    mCodecId = codecId;

    if( mCodecId != CharCoding
        && mCodecId != Utf8Coding )
    {
        delete mValueCodec;
        mValueCodec = ValueCodec::createCodec( (Okteta::ValueCoding)mCodecId );
    }
}

void ByteArrayValidator::setMaxLength( int maxLength )
{
    mMaxLength = maxLength;
    if( maxLength < mMinLength )
        mMinLength = maxLength;
}

void ByteArrayValidator::setMinLength( int minLength )
{
    mMinLength = minLength;
    if( minLength > mMaxLength )
        mMaxLength = minLength;
}

QValidator::State ByteArrayValidator::validate( QString& string, int& pos ) const
{
    Q_UNUSED( pos )

    State result = QValidator::Acceptable;

    int stringLength = string.length();

    if( mCodecId == CharCoding )
    {
        if( stringLength > mMaxLength )
        {
            string.truncate( mMaxLength );
            stringLength = mMaxLength;
        }

        for( int i=0; i<stringLength; ++i )
        {
            const QChar c = string.at( i );
            if( !mCharCodec->canEncode(c) )
            {
                result = QValidator::Invalid;
                break;
            }
        }
    }
    else if( mCodecId != Utf8Coding )
    {
        const int encodingWidth = mValueCodec->encodingWidth();
        int byteCount = 0;
        for( int i = 0; i < stringLength; )
        {
            Okteta::Byte dummyByte;
            const int usedCharCount = mValueCodec->decode( &dummyByte, string, i );

            // could not decode?
            if( usedCharCount == 0 )
            {
                result = QValidator::Invalid;
                break;
            }
            i += usedCharCount;
            ++byteCount;

            if( byteCount >= mMaxLength )
            {
                string.truncate( i );
                break;
            }
        }
        if( byteCount < mMinLength )
        {
            const int paddingCount = (mMinLength-byteCount) * encodingWidth;
            string += QString( paddingCount, QLatin1Char('0') );
        }
    }

    return result;
}


QByteArray ByteArrayValidator::toByteArray( const QString& string ) const
{
    QByteArray result;

    const int stringLength = string.length();
    if( mCodecId == CharCoding )
    {
        result.resize( stringLength );
        for( int i=0; i<stringLength; ++i )
        {
            Byte byte;
            const bool success = mCharCodec->encode( &byte, string[i] );
            result[i] = success ? byte : '?'; // TODO: define unknown symbol
        }
    }
    else if( mCodecId == Utf8Coding )
        result = string.toUtf8();
    else
    {
        int i = 0;
        while( i < stringLength )
        {
            Byte byte;
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


QString ByteArrayValidator::toString( const QByteArray& byteArray ) const
{
    QString result;

    const int byteArraySize = byteArray.size();
    if( mCodecId == Utf8Coding )
        result = QString::fromUtf8( byteArray.constData(), byteArraySize );
    else if( mCodecId == CharCoding )
    {
        result.resize( byteArraySize );
        for( int i=0; i<byteArraySize; ++i )
        {
            Character c = mCharCodec->decode( byteArray[i] );
            result[i] = c.isUndefined() ? QChar::fromLatin1('?') : c; // TODO: define unknown symbol
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


ByteArrayValidator::~ByteArrayValidator()
{
    delete mValueCodec;
    delete mCharCodec;
}

}
