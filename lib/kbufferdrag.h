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


#ifndef KHE_KBUFFERDRAG_H
#define KHE_KBUFFERDRAG_H

// qt specific
#include <qdragobject.h>
// lib specific
#include "khe.h"
#include "kcoordrange.h"
#include "kcoltextexport.h"

namespace KHE
{

// class KBorderColumn;
class KOffsetColumn;
class KValueColumn;
class KCharColumn;

typedef KColTextExport* KColTextExportPtr;
/**
  *@author Friedrich W. H. Kossebau
  */
class KBufferDrag : public QDragObject
{
    Q_OBJECT

  public:
    // TODO: make this call somewhat more generic
    KBufferDrag( const QByteArray &, KCoordRange Range,
                 const KOffsetColumn *OC, const KValueColumn *HC, const KCharColumn *TC,
                 QChar SC, QChar UC, const QString &CN,
                 QWidget *Source = 0, const char *Name = 0 );
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
    KColTextExportPtr Columns[5];
    uint NoOfCol;
    QChar SubstituteChar;
    QChar UndefinedChar;
    const QString &CodecName;
};

}

#endif
