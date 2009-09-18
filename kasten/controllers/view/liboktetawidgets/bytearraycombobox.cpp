/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraycombobox.h"
#include "bytearraycombobox_p.h"
#include "bytearraycombobox.moc"


namespace Okteta
{

ByteArrayComboBox::ByteArrayComboBox( QWidget* parent )
  : QWidget( parent ),
    d_ptr( new ByteArrayComboBoxPrivate(this) )
{
    Q_D( ByteArrayComboBox );

    d->init();
}

void ByteArrayComboBox::setCharCodec( const QString& charCodecName )
{
    Q_D( ByteArrayComboBox );

    d->setCharCodec( charCodecName );
}

QByteArray ByteArrayComboBox::data() const
{
    Q_D( const ByteArrayComboBox );

    return d->data();
}

int ByteArrayComboBox::format() const
{
    Q_D( const ByteArrayComboBox );

    return d->format();
}

ByteArrayComboBox::~ByteArrayComboBox()
{
    delete d_ptr;
}

}
