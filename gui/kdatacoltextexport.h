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

#ifndef KHE_UI_KDATACOLTEXTEXPORT_H
#define KHE_UI_KDATACOLTEXTEXPORT_H

// lib
#include "kcoltextexport.h"
#include "kcoordrange.h"
// Qt
#include <QtCore/QString>


namespace KHEUI
{

class AbstractByteArrayColumnRenderer;


class KDataColTextExport : public KColTextExport
{
  public:
    KDataColTextExport( const AbstractByteArrayColumnRenderer* BF, const char *D, KCoordRange CR, int BytesWidth );
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
