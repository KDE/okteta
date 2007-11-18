/***************************************************************************
                          kbytearraydocumentfactory.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kbytearraydocumentfactory.h"

// lib
#include "kbytearraydocument.h"
// KDE
#include <KLocale>


static int newByteArrayDocumentCounter = 0;


KAbstractDocument *KByteArrayDocumentFactory::create()
{
    KByteArrayDocument *document = new KByteArrayDocument();

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]","[New Byte Array %1]",newByteArrayDocumentCounter) );

    return document;
}
