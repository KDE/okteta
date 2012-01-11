/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraybase32streamencoderconfigeditorfactory.h"

// lib
#include "bytearraybase32streamencoderconfigeditor.h"
#include "bytearraybase32streamencoder.h"


namespace Kasten2
{

ByteArrayBase32StreamEncoderConfigEditorFactory::ByteArrayBase32StreamEncoderConfigEditorFactory()
{
}


AbstractModelStreamEncoderConfigEditor* ByteArrayBase32StreamEncoderConfigEditorFactory::tryCreateConfigEditor( AbstractModelStreamEncoder* encoder ) const
{
    AbstractModelStreamEncoderConfigEditor* result = 0;
    ByteArrayBase32StreamEncoder* base32StreamEncoder =
        qobject_cast<ByteArrayBase32StreamEncoder*>( encoder );

    if( base32StreamEncoder )
        result = new ByteArrayBase32StreamEncoderConfigEditor( base32StreamEncoder );

    return result;
}


ByteArrayBase32StreamEncoderConfigEditorFactory::~ByteArrayBase32StreamEncoderConfigEditorFactory() {}

}
