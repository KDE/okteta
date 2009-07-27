/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearraydocumentfactory.h"

// lib
#include "kbytearraydocument.h"
// Kasten core
#include <person.h>
// KDE
#include <KLocale>


namespace Kasten
{

static int newByteArrayDocumentCounter = 0;


AbstractDocument* KByteArrayDocumentFactory::create()
{
    KByteArrayDocument *document = new KByteArrayDocument( i18nc("The byte array was new created.","New created.") );

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]","[New Byte Array %1]",newByteArrayDocumentCounter) );

    document->setOwner( Person::createEgo() );

    return document;
}

}
