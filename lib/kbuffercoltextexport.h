/***************************************************************************
                          kbuffercoltextexport.h  -  description
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


#ifndef KHE_KBUFFERCOLTEXTEXPORT_H
#define KHE_KBUFFERCOLTEXTEXPORT_H

// qt specific
#include <qstring.h>
// lib specific
#include "kcoltextexport.h"
#include "kcoordrange.h"


namespace KHE
{

class KBufferColumn;


class KBufferColTextExport : public KColTextExport
{
  public:
    KBufferColTextExport( const KBufferColumn* BF, const char *D, KCoordRange CR, int BytesWidth );
    virtual ~KBufferColTextExport();

  public: // API
    void printFirstLine( QString &T, int Line ) const;
    void printNextLine( QString &T ) const;
    /** tells how much chars per line are needed */
    int charsPerLine() const;


  protected: // API to be reimplemented by subclasses
    virtual void print( QString &T ) const;


  protected:
    static QString whiteSpace( uint s );

  protected:
    const char *Data;
    KCoordRange CoordRange;

    int NoOfBytesPerLine;

    /** Line to print */
    mutable int PrintLine;
    /** Data to print */
    mutable const char *PrintData;

    /** buffered value of how many chars a line needs */
    int NoOfCharsPerLine;
    // positions where to paint the
    int *Pos;
};

}

#endif
