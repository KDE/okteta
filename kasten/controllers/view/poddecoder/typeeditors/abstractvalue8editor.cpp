/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "abstractvalue8editor.h"

// Okteta core
#include <valuecodec.h>
// C++
#include <limits>


AbstractValue8Editor::AbstractValue8Editor( Okteta::ValueCoding valueCoding, QWidget* parent )
  : QSpinBox( parent )
{
    const quint8 int8Max = std::numeric_limits<quint8>::max();
    const quint8 int8Min = std::numeric_limits<quint8>::min();

    setRange( int8Min, int8Max );

    mValueCodec = Okteta::ValueCodec::createCodec( valueCoding );
}

QString AbstractValue8Editor::textFromValue( int value ) const
{
    QString result;

    mValueCodec->encodeShort( result, 0, (Okteta::Byte)value );

    return result;
}

int AbstractValue8Editor::valueFromText( const QString& text ) const
{
    Okteta::Byte byte;
    mValueCodec->decode( &byte, text );

    return byte;
}

QValidator::State AbstractValue8Editor::validate( QString& text, int& pos ) const
{
    Q_UNUSED( pos );

    Okteta::Byte byte;
    const int textLength = text.length();
    const int usedChars = mValueCodec->decode( &byte, text );

    const bool isCompleteTextUsed = ( textLength == usedChars );

    return isCompleteTextUsed ? QValidator::Acceptable : QValidator::Invalid;
}

AbstractValue8Editor::~AbstractValue8Editor()
{
    delete mValueCodec;
}
