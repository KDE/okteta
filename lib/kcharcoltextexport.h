/***************************************************************************
                          ktextcoltextexport.h  -  description
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


#ifndef KTEXTCOLTEXTEXPORT_H
#define KTEXTCOLTEXTEXPORT_H

#include "kbuffercoltextexport.h"


namespace KHE
{

class KTextColumn;


class KTextColTextExport : public KBufferColTextExport
{
  public:
    KTextColTextExport( const KTextColumn* BF, char *D, KCoordRange CR );
    virtual ~KTextColTextExport();

  protected: //API
    virtual void print( char **T ) const;

  protected:
    char SubstituteChar;
};

}

#endif
