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

    const QString title( (newByteArrayDocumentCounter>0) ?
             i18n( "new %1", newByteArrayDocumentCounter ) :
             i18n( "new" ) );
    document->setTitle( title );

    ++newByteArrayDocumentCounter;

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
