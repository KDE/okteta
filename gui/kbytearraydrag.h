/***************************************************************************
                          kbytearraydrag.h  -  description
                             -------------------
    begin                : Mon Jul 07 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KDATADRAG_H
#define KHE_CORE_KDATADRAG_H

// lib
#include "khe.h"
#include "kcoordrange.h"
#include "kcoltextexport.h"
// Qt
#include <QtCore/QMimeData>


namespace KHEUI
{

// class KBorderColumn;
class KOffsetColumn;
class KValueColumn;
class KCharColumn;

typedef KColTextExport* KColTextExportPtr;

/**
  *@author Friedrich W. H. Kossebau
  */
class KByteArrayDrag : public QMimeData
{
  public:
    // TODO: make this call somewhat more generic
    KByteArrayDrag( const QByteArray &, const KCoordRange &Range,
                 const KOffsetColumn *OC, const KValueColumn *HC, const KCharColumn *TC,
                 QChar SC, QChar UC, const QString &CN );
    virtual ~KByteArrayDrag();

  public: // QMimeData API
    virtual QStringList formats() const;
    //virtual bool hasFormat( const QString &Mimetype ) const;
    virtual QVariant retrieveData( const QString &mimetype, QVariant::Type type ) const;

  protected:
    QString createTextCopy() const;
    QString createColumnCopy() const;

  protected:
    QByteArray BufferCopy;

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
