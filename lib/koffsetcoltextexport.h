/***************************************************************************
                          koffsetcoltextexport.h  -  description
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


#ifndef KHE_KOFFSETCOLTEXTEXPORT_H
#define KHE_KOFFSETCOLTEXTEXPORT_H

// lib specific
#include "kcoltextexport.h"
#include "koffsetformat.h"


namespace KHE
{

class KOffsetColumn;

class KOffsetColTextExport : public KColTextExport
{
  public:
    KOffsetColTextExport( const KOffsetColumn *OC );

  public: // KColTextExport API
    void printFirstLine( QString &T, int Line ) const;
    void printNextLine( QString &T ) const;
    /** tells how much chars per line are needed */
    int charsPerLine() const;

  protected:
    void print( QString &T ) const;

  protected:
    const int CodingWidth;
    const int FirstLineOffset;
    const int Delta;
    const KOffsetFormat::print printFunction;

    /** the line we are in */
    mutable int PrintLine;
};

}

#endif
