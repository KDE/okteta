/***************************************************************************
                          kbordercoltextexport.h  -  description
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


#ifndef KHE_KBORDERCOLTEXTEXPORT_H
#define KHE_KBORDERCOLTEXTEXPORT_H

#include "kcoltextexport.h"

namespace KHE
{

class KBorderColTextExport : public KColTextExport
{
  public: // API
    void printFirstLine( char **T, int Line ) const;
    void printNextLine( char **T ) const;
    /** tells how much chars per line are needed */
    int charsPerLine() const ;

  protected:
    void print( char **T ) const;
};

}

#endif
