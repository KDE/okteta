/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KHE_UI_KBYTEARRAYDRAG_H
#define KHE_UI_KBYTEARRAYDRAG_H

// lib
#include "khe.h"
#include "kcoordrange.h"
#include "kcoltextexport.h"
// Qt
#include <QtCore/QMimeData>


namespace KHEUI
{

class OffsetColumnRenderer;
class ValueByteArrayColumnRenderer;
class CharByteArrayColumnRenderer;

typedef KColTextExport* KColTextExportPtr;

/**
  *@author Friedrich W. H. Kossebau
  */
class KByteArrayDrag : public QMimeData
{
  public:
    // TODO: make this call somewhat more generic
    KByteArrayDrag( const QByteArray &, const KCoordRange &Range,
                 const OffsetColumnRenderer *OC, const ValueByteArrayColumnRenderer *HC, const CharByteArrayColumnRenderer *TC,
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
