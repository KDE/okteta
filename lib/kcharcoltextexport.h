/***************************************************************************
                          kcharcoltextexport.h  -  description
                             -------------------
    begin                : Wed Sep 3 2003
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


#ifndef KCHARCOLTEXTEXPORT_H
#define KCHARCOLTEXTEXPORT_H

#include "kbuffercoltextexport.h"


namespace KHE
{

class KCharColumn;


class KCharColTextExport : public KBufferColTextExport
{
  public:
    KCharColTextExport( const KCharColumn* BF, char *D, KCoordRange CR );
    virtual ~KCharColTextExport();

  protected: //API
    virtual void print( char **T ) const;

  protected:
    char SubstituteChar;
};

}

#endif
