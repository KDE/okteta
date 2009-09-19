/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "addressvalidator.h"

// Okteta core
#include <valuecodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QString>


namespace Okteta
{

AddressValidator::AddressValidator( QObject* parent, Coding codecId )
  : QValidator( parent ),
    mCodecId( InvalidCoding ),
    mValueCodec( 0 )
{
    setCodec( codecId );
}

void AddressValidator::setCodec( Coding codecId )
{
    if( codecId == mCodecId )
        return;

    mCodecId = codecId;

    delete mValueCodec;
    mValueCodec = ValueCodec::createCodec( (Okteta::ValueCoding)mCodecId );
}

QValidator::State AddressValidator::validate( QString& string, int& pos ) const
{
    Q_UNUSED( pos )

    State result = QValidator::Acceptable;

    const int stringLength = string.length();
    for( int i=0; i<stringLength; ++i )
    {
        const QChar c = string.at( i );
        if( !mValueCodec->isValidDigit(c.toLatin1()) && !c.isSpace() )
        {
            result = QValidator::Invalid;
            break;
        }
    }

    return result;
}


Address AddressValidator::toAddress( const QString& string ) const
{
    const int isHexadecimal = ( mCodecId == HexadecimalCoding );
    const int base = isHexadecimal ? 16 : 10;
    const Okteta::Address address = string.toInt( 0, base );

    return address;
}

QString AddressValidator::toString( Address address ) const
{
    const int isHexadecimal = ( mCodecId == HexadecimalCoding );
    const int base = isHexadecimal ? 16 : 10;
    const QString string = QString::number( address, base );
    return string;
}

AddressValidator::~AddressValidator()
{
    delete mValueCodec;
}

}
