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

#include "bytearrayfilterparameterseteditfactory.h"

// lib
#include "filter/nobytearrayfilterparametersetedit.h"
#include "filter/operandbytearrayfilterparametersetedit.h"
#include "filter/reversebytearrayfilterparametersetedit.h"
#include "filter/rotatebytearrayfilterparametersetedit.h"


AbstractByteArrayFilterParameterSetEdit* ByteArrayFilterParameterSetEditFactory::createEdit( const char* id )
{
    AbstractByteArrayFilterParameterSetEdit* result;

    if( qstrcmp(id,"Operand") == 0 )
        result = new OperandByteArrayFilterParameterSetEdit();
    else if( qstrcmp(id,"Reverse") == 0 )
        result = new ReverseByteArrayFilterParameterSetEdit();
    else if( qstrcmp(id,"Rotate") == 0 )
        result = new RotateByteArrayFilterParameterSetEdit();
    else //if( qstrcmp(id,"None") == 0 ) TODO: default should be a message "Not found"
        result = new NoByteArrayFilterParameterSetEdit();

    return result;
}
