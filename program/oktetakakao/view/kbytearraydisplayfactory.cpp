/***************************************************************************
                          kviewfactory.h  -  description
                             -------------------
    begin                : Thu Nov 2 2006
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


#include "kbytearraydisplayfactory.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearraydisplay.h"


KAbstractView *KByteArrayDisplayFactory::createViewFor( KAbstractDocument *D )
{
    KByteArrayDisplay *result = 0;

    KByteArrayDocument *document = static_cast<KByteArrayDocument*>( D );
    if( document )
        result = new KByteArrayDisplay( document );

    return result;
}
