/***************************************************************************
                          kvaluecoltextexport.h  -  description
                             -------------------
    begin                : Sam Aug 30 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KVALUECOLTEXTEXPORT_H
#define KVALUECOLTEXTEXPORT_H

#include "kbytecodec.h"
#include "kbuffercoltextexport.h"


namespace KHE
{

class KValueColumn;


class KValueColTextExport : public KBufferColTextExport
{
  public:
    KValueColTextExport( const KValueColumn* BF, char *D, KCoordRange CR );
    virtual ~KValueColTextExport();

  protected:
    virtual void print( char **T ) const;


  protected:
    int CodingWidth;
    KByteCodec::coding CodingFunction;
};

}

#endif
