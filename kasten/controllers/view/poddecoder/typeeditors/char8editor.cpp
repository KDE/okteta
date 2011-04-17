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

#include "char8editor.h"

// Okteta core
#include <character.h>
#include <charcodec.h>
// Qt
#include <QtGui/QValidator>


class Char8Validator : public QValidator
{
  public:
    Char8Validator( Okteta::CharCodec* charCodec, QObject* parent = 0 );

    virtual ~Char8Validator();

  public: // QValidator API
    virtual QValidator::State validate( QString& input, int& pos ) const;

  protected:
    Okteta::CharCodec* mCharCodec;
};

inline Char8Validator::Char8Validator( Okteta::CharCodec* charCodec, QObject* parent )
  : QValidator( parent ),
    mCharCodec( charCodec )
{}


QValidator::State Char8Validator::validate( QString& input, int& pos ) const
{
    Q_UNUSED( pos )

    State result;

    const int stringLength = input.length();
    if( stringLength == 0 )
        result = QValidator::Intermediate;
    else
    {
        const QChar c = input.at( 0 );

        result = mCharCodec->canEncode( c ) ? QValidator::Acceptable : QValidator::Invalid;
    }

    return result;
}

Char8Validator::~Char8Validator() {}


Char8Editor::Char8Editor( Okteta::CharCodec* charCodec, QWidget* parent )
  : KLineEdit( parent )
{
    setValidator( new Char8Validator(charCodec,this) );
    setMaxLength( 1 );
}

void Char8Editor::setData( Char8 data )
{
    setText( data.character.isUndefined() ? QString() : QString(data.character) );
}

Char8 Char8Editor::data() const
{
    const QString t = text();
    return Char8( t.isEmpty() ? QChar(0) : t[0] );
}

Char8Editor::~Char8Editor() {}
