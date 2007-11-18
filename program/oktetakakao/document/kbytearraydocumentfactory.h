/***************************************************************************
                          kbytearraydocumentfactory.h  -  description
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


#ifndef KBYTEARRAYDOCUMENTFACTORY_H
#define KBYTEARRAYDOCUMENTFACTORY_H


// kakao
#include <kabstractdocumentfactory.h>


class KByteArrayDocumentFactory : public KAbstractDocumentFactory
{
    Q_OBJECT

  public: // KAbstractDocumentFactory API
    virtual KAbstractDocument *create();
};

#endif
