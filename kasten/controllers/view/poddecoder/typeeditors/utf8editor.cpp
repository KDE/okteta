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

#include "utf8editor.h"

// Qt
#include <QtGui/QValidator>


class Utf8CharValidator : public QValidator
{
  public:
    explicit Utf8CharValidator( QObject* parent = 0 );

    virtual ~Utf8CharValidator();

  public: // QValidator API
    virtual QValidator::State validate( QString& input, int& pos ) const;
};

inline Utf8CharValidator::Utf8CharValidator( QObject* parent ) : QValidator( parent ) {}


QValidator::State Utf8CharValidator::validate( QString& input, int& pos ) const
{
    Q_UNUSED( pos )

    const int stringLength = input.length();
    return ( stringLength == 0 ) ? QValidator::Intermediate : QValidator::Acceptable;
}

Utf8CharValidator::~Utf8CharValidator() {}


Utf8Editor::Utf8Editor( QWidget* parent )
  : KLineEdit( parent )
{
    setValidator( new Utf8CharValidator(this) );
    setMaxLength( 1 );
}

void Utf8Editor::setData( Utf8 data )
{
    setText( data.value );
}

Utf8 Utf8Editor::data() const
{
    const QString t = text();
    return Utf8( t.isEmpty() ? QChar(0) : t[0] );
}

Utf8Editor::~Utf8Editor() {}
