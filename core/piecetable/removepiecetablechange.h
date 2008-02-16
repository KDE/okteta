/***************************************************************************
                          removepiecetablechange.h  -  description
                             -------------------
    begin                : Mon Feb 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KPIECETABLE_REMOVEPIECETABLECHANGE_H
#define KPIECETABLE_REMOVEPIECETABLECHANGE_H


// lib
#include "abstractpiecetablechange.h"
#include "piecelist.h"

namespace KPieceTable
{

/** class
  *@author Friedrich W. H. Kossebau
  */

class RemovePieceTableChange : public AbstractPieceTableChange
{
  public:
    RemovePieceTableChange( const KHE::KSection &removeSection, const PieceList &removedPieces );
    virtual ~RemovePieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual bool merge( const AbstractPieceTableChange *other );
    virtual KHE::KSection apply( PieceTable *pieceTable );
    virtual KHE::KSection revert( PieceTable *pieceTable );
    virtual KHE::ReplacementScope replacement() const;

  protected:
    KHE::KSection mRemoveSection;
    PieceList mRemovedPieces;
};

inline RemovePieceTableChange::RemovePieceTableChange( const KHE::KSection &removeSection, const PieceList &removedPieces )
 : mRemoveSection( removeSection ), mRemovedPieces( removedPieces ) {}
}

#endif
