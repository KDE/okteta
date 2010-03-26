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
#include <KDebug>
// Qt
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>


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
    if ( codecId == mCodecId )
        return;

    mCodecId = codecId;
    delete mValueCodec;
    mValueCodec = ValueCodec::createCodec( (Okteta::ValueCoding)mCodecId );
}

const QRegExp AddressValidator::expressionRegex = QRegExp("[0-9a-fx\\+/\\s\\-\\*]+",
        Qt::CaseInsensitive, QRegExp::RegExp2); //FIXME this is way too simple, only there to test

QValidator::State AddressValidator::validate( QString& string, int& pos ) const
{
    Q_UNUSED( pos )

    State result = QValidator::Acceptable;
    if ( mCodecId == ExpressionCoding )
    {
        string = string.trimmed();
        if ( !expressionRegex.exactMatch( string ) )
            result = QValidator::Invalid;
        //only prefix has been typed:
        if( string == QLatin1String("+")
            || string == QLatin1String("-")
            || string.endsWith( 'x' ) ) // 0x at end
            result = QValidator::Intermediate;
    }
    else
    {
        const int stringLength = string.length();
        for( int i=0; i<stringLength; ++i )
        {
            const QChar c = string.at( i );
            if( !mValueCodec->isValidDigit( c.toLatin1() ) && !c.isSpace() )
            {
                result = QValidator::Invalid;
                break;
            }
        }
    }
    if ( string.isEmpty() )
        result = QValidator::Intermediate;
    return result;
}

Address AddressValidator::toAddress(const QString& string, AddressType* type) const
{
    Address address;
    QString expression = string.trimmed();
    if (type) // allow passing null
        *type = calculateAddressType(expression);

    if ( expression.startsWith( '-' ) || expression.startsWith( '+' ) )
        expression.remove( 0, 1 );

    if ( mCodecId == ExpressionCoding )
    {
        QScriptEngine evaluator;
        QScriptValue val = evaluator.evaluate( expression );
        address = val.toInt32();
        kDebug() << "expression \"" << expression << "\" evaluated to: " << address;
        if ( evaluator.hasUncaughtException() )
        {
            kWarning() << "evaluation error: "
                    << evaluator.uncaughtExceptionBacktrace();
            if (type)
                *type = InvalidAddressType;
        }
    }
    else
    {
        const bool isHexadecimal = ( mCodecId == HexadecimalCoding );
        const int base = isHexadecimal ? 16 : 10;
        address = expression.toInt( 0, base );
    }
    return address;
}

AddressValidator::AddressType AddressValidator::calculateAddressType( const QString& string ) const
{
    AddressType result;
    QString expression = string.trimmed();
    if ( expression.startsWith( '+' ) )
    {
        expression.remove( 0, 1 );
        result = RelativeForwards;
    }
    else if ( expression.startsWith( '-' ) )
    {
        expression.remove( 0, 1 );
        result = RelativeBackwards;
    }
    else
    {
        result = AbsoluteAddress;
    }
    return result;
}

QString AddressValidator::toString( Address address, const AddressType* addressType ) const
{
    //ExpressionCoding just uses base 10 so no need to adjust this code
    const int isHexadecimal = ( mCodecId == HexadecimalCoding );
    const int base = isHexadecimal ? 16 : 10;
    QString string = QString::number( address, base );

    if (addressType)
    {
        if ( *addressType == RelativeForwards )
            string = string.prepend( '+' );
        else if ( *addressType == RelativeBackwards )
            string = string.prepend( '-' );
    }
    return string;
}

AddressValidator::~AddressValidator()
{
    delete mValueCodec;
}

}
