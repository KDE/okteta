/***************************************************************************
                          kdocumentcontroller.h  -  description
                             -------------------
    begin                : Mon Oct 15 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDOCUMENTCONTROLLER_H
#define KDOCUMENTCONTROLLER_H


// lib
#include "kcontroller.h"

class KAbstractDocument;

class KDocumentController : public KController
{
  public: // API to be implemented
    virtual void setDocument( KAbstractDocument * ) = 0;
};

#endif
