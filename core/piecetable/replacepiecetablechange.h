/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KPIECETABLE_REPLACEPIECETABLECHANGE_H
#define KPIECETABLE_REPLACEPIECETABLECHANGE_H


// lib
#include "abstractpiecetablechange.h"
#include "piecelist.h"

namespace KPieceTable
{

/** class
  *@author Friedrich W. H. Kossebau
  */

class ReplacePieceTableChange : public AbstractPieceTableChange
{
  public:
    ReplacePieceTableChange( const KHE::Section &removeSection, int insertLength, int storageOffset,
                             const PieceList &removedPieces );
    virtual ~ReplacePieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual int storageOffset() const;
    virtual bool merge( const AbstractPieceTableChange *other );
    virtual KHE::Section apply( PieceTable *pieceTable ) const;
    virtual KHE::Section revert( PieceTable *pieceTable ) const;
    virtual KHE::ArrayChangeMetrics metrics() const;
    virtual int dataSize() const;

  protected:
    KHE::Section mRemoveSection;
    int mInsertLength;
    int mStorageOffset;
    PieceList mRemovedPieces;
};

inline ReplacePieceTableChange::ReplacePieceTableChange( const KHE::Section &removeSection,
                                                         int insertLength, int storageOffset,
                                                         const PieceList &removedPieces )
 : mRemoveSection( removeSection ),
   mInsertLength( insertLength ), mStorageOffset( storageOffset ),
   mRemovedPieces( removedPieces )
{}

}

#endif
