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

#include "bytearrayfilterparameterseteditfactory.h"

// lib
#include "filter/nobytearrayfilterparametersetedit.h"
#include "filter/operandbytearrayfilterparametersetedit.h"
#include "filter/reversebytearrayfilterparametersetedit.h"
#include "filter/rotatebytearrayfilterparametersetedit.h"
//// NEWFILTERPARAMETERSET(start)
//// Here add the name of your header file of your edit widget for the parameterset,
//// e.g.
//// #include "filter/my_bytearrayfilterparametersetedit.h"
//// NEWFILTERPARAMETERSET(end)


AbstractByteArrayFilterParameterSetEdit* ByteArrayFilterParameterSetEditFactory::createEdit( const char* id )
{
    AbstractByteArrayFilterParameterSetEdit* result;

    if( qstrcmp(id,OperandByteArrayFilterParameterSetEdit::Id) == 0 )
        result = new OperandByteArrayFilterParameterSetEdit();
    else if( qstrcmp(id,ReverseByteArrayFilterParameterSetEdit::Id) == 0 )
        result = new ReverseByteArrayFilterParameterSetEdit();
    else if( qstrcmp(id,RotateByteArrayFilterParameterSetEdit::Id) == 0 )
        result = new RotateByteArrayFilterParameterSetEdit();
//// NEWFILTERPARAMETERSET(start)
//// Here add the check for the id of your parameter set
//// and if it matches the creation of the widget
//// e.g.
////     else if( qstrcmp(id,My_ByteArrayFilterParameterSetEdit::Id) == 0 )
////         result = new My_ByteArrayFilterParameterSetEdit();
//// NEWFILTERPARAMETERSET(end)
    else //if( qstrcmp(id,NoByteArrayFilterParameterSetEdit::Id) == 0 ) TODO: default should be a message "Not found"
        result = new NoByteArrayFilterParameterSetEdit();

    return result;
}
