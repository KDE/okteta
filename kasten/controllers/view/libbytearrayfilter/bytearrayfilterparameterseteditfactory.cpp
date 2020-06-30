/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayfilterparameterseteditfactory.hpp"

// lib
#include "filter/nobytearrayfilterparametersetedit.hpp"
#include "filter/operandbytearrayfilterparametersetedit.hpp"
#include "filter/reversebytearrayfilterparametersetedit.hpp"
#include "filter/rotatebytearrayfilterparametersetedit.hpp"
//// NEWFILTERPARAMETERSET(start)
//// Here add the name of your header file of your edit widget for the parameterset,
//// e.g.
//// #include "filter/my_bytearrayfilterparametersetedit.hpp"
//// NEWFILTERPARAMETERSET(end)

AbstractByteArrayFilterParameterSetEdit* ByteArrayFilterParameterSetEditFactory::createEdit(const char* id)
{
    AbstractByteArrayFilterParameterSetEdit* result;

    if (qstrcmp(id, OperandByteArrayFilterParameterSetEdit::Id) == 0) {
        result = new OperandByteArrayFilterParameterSetEdit();
    } else if (qstrcmp(id, ReverseByteArrayFilterParameterSetEdit::Id) == 0) {
        result = new ReverseByteArrayFilterParameterSetEdit();
    } else if (qstrcmp(id, RotateByteArrayFilterParameterSetEdit::Id) == 0) {
        result = new RotateByteArrayFilterParameterSetEdit();
    }
//// NEWFILTERPARAMETERSET(start)
//// Here add the check for the id of your parameter set
//// and if it matches the creation of the widget
//// e.g.
////     else if (qstrcmp(id, My_ByteArrayFilterParameterSetEdit::Id) == 0)
////         result = new My_ByteArrayFilterParameterSetEdit();
//// NEWFILTERPARAMETERSET(end)
    else { // if (qstrcmp(id, NoByteArrayFilterParameterSetEdit::Id) == 0) TODO: default should be a message "Not found"
        result = new NoByteArrayFilterParameterSetEdit();
    }

    return result;
}
