/***************************************************************************
                          khexcoltextexport.h  -  description
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


#ifndef KHEXCOLTEXTEXPORT_H
#define KHEXCOLTEXTEXPORT_H

#include "kbytecodec.h"
#include "kbuffercoltextexport.h"


namespace KHE
{

class KHexColumn;


class KHexColTextExport : public KBufferColTextExport
{
  public:
    KHexColTextExport( const KHexColumn* BF, char *D, KCoordRange CR );
    virtual ~KHexColTextExport();

  protected:
    virtual void print( char **T ) const;


  protected:

    int CodingWidth;
    KByteCodec::coding CodingFunction;
};

}

#endif
