/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychecksumparameterseteditfactory.hpp"

// lib
#include "algorithm/crc64bytearraychecksumparametersetedit.hpp"
#include "algorithm/modsumbytearraychecksumparametersetedit.hpp"
#include "algorithm/nobytearraychecksumparametersetedit.hpp"
//// NEWCHECKSUMPARAMETERSET(start)
//// Here add the name of your header file of your edit widget for the parameterset,
//// e.g.
//// #include "algorithm/mybytearraychecksumparametersetedit.hpp"
//// NEWCHECKSUMPARAMETERSET(end)

std::unique_ptr<AbstractByteArrayChecksumParameterSetEdit> ByteArrayChecksumParameterSetEditFactory::createEdit(const char* id)
{
    std::unique_ptr<AbstractByteArrayChecksumParameterSetEdit> result;

    if (qstrcmp(id, ModSumByteArrayChecksumParameterSetEdit::Id) == 0) {
        result = std::make_unique<ModSumByteArrayChecksumParameterSetEdit>();
    } else if (qstrcmp(id, Crc64ByteArrayChecksumParameterSetEdit::Id) == 0) {
        result = std::make_unique<Crc64ByteArrayChecksumParameterSetEdit>();
    }
//// NEWCHECKSUMPARAMETERSET(start)
//// Here add the check for the id of your parameter set
//// and, if it matches, the creation of the widget
//// e.g.
////     else if (qstrcmp(id, MyByteArrayChecksumParameterSetEdit::Id) == 0)
////         result = std::make_unique<MyByteArrayChecksumParameterSetEdit>();
//// NEWCHECKSUMPARAMETERSET(end)
    else { // if (qstrcmp(id, NoByteArrayChecksumParameterSetEdit::Id) == 0) TODO: default should be a message "Not found"
        result = std::make_unique<NoByteArrayChecksumParameterSetEdit>();
    }

    return result;
}
