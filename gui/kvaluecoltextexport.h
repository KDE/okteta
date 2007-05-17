/***************************************************************************
                          kvaluecoltextexport.h  -  description
                             -------------------
    begin                : Sam Aug 30 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KVALUECOLTEXTEXPORT_H
#define KHE_UI_KVALUECOLTEXTEXPORT_H

// lib
#include "kdatacoltextexport.h"
// hexedit core
#include "kbytecodec.h"


namespace KHEUI
{

class KValueColumn;


class KValueColTextExport : public KDataColTextExport
{
  public:
    KValueColTextExport( const KValueColumn* BF, const char *D, const KCoordRange &CR );
    virtual ~KValueColTextExport();

  protected:
    virtual void print( QString *T ) const;


  protected:
    KHECore::KByteCodec *ByteCodec;
};

}

#endif
