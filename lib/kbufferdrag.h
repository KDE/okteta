/***************************************************************************
                          kbufferdrag.h  -  description
                             -------------------
    begin                : Mon Jul 07 2003
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


#ifndef KBUFFERDRAG_H
#define KBUFFERDRAG_H

// qt specific
#include <qdragobject.h>
// app specific
#include "kcoordrange.h"
#include "kcoltextexport.h"

namespace KHE
{

// class KBorderColumn;
class KOffsetColumn;
class KHexColumn;
class KTextColumn;

typedef KColTextExport* KColTextExportPtr;
/**
  *@author Friedrich W. H. Kossebau
  */
class KBufferDrag : public QDragObject
{
    Q_OBJECT

  public:
    static const char *OctetStream;
    static const char *PlainText;

  public:
    // TODO: make this call somewhat more generic
    KBufferDrag( const QByteArray &, KCoordRange Range, const KOffsetColumn *OC, const KHexColumn *HC, const KTextColumn *TC,
                 char SC,
                 QWidget *Source = 0, const char *Name = 0 );
    KBufferDrag( QWidget *Source = 0, const char *Name = 0 );
    ~KBufferDrag();

  public: // QDragObject API
    virtual const char *format( int i ) const;
    virtual QByteArray encodedData( const char* ) const;

  public:
    virtual void setData( const QByteArray &);


  public:
    static bool canDecode( const QMimeSource* Source );
    static bool decode( const QMimeSource* Source, QByteArray &Dest );


  protected:
    QByteArray Data;
    KCoordRange CoordRange;
    /** collection of all the columns. All columns will be autodeleted. */
    KColTextExportPtr* Columns;
    int NoOfCol;
    char SubstituteChar;
};

}

#endif
