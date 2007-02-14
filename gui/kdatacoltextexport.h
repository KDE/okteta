/***************************************************************************
                          kdatacoltextexport.h  -  description
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


#ifndef KHE_UI_KDATACOLTEXTEXPORT_H
#define KHE_UI_KDATACOLTEXTEXPORT_H

// qt specific
#include <QtCore/QString>
// lib specific
#include "kcoltextexport.h"
#include "kcoordrange.h"


namespace KHEUI
{

class KDataColumn;


class KDataColTextExport : public KColTextExport
{
  public:
    KDataColTextExport( const KDataColumn* BF, const char *D, KCoordRange CR, int BytesWidth );
    virtual ~KDataColTextExport();

  public: // API
    void printFirstLine( QString *T, int Line ) const;
    void printNextLine( QString *T ) const;
    /** tells how much chars per line are needed */
    int charsPerLine() const;


  protected: // API to be reimplemented by subclasses
    virtual void print( QString *T ) const;


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
