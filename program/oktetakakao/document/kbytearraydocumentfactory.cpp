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

    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]","[New Byte Array %1]",newByteArrayDocumentCounter) );

    return document;
}


KAbstractDocument *KByteArrayDocumentFactory::load( const QString &localFileName, const QString &OriginUrl )
{
    KByteArrayDocument *document = new KByteArrayDocument();

    const bool success = document->load(localFileName);
    if( success )
        document->setUrl( OriginUrl );
    else
    {
        delete document;
        document = 0;
    }
    return document;
}
