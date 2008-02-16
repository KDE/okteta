/***************************************************************************
                          insertpiecetablechange.h  -  description
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


#ifndef KPIECETABLE_INSERTPIECETABLECHANGE_H
#define KPIECETABLE_INSERTPIECETABLECHANGE_H


// lib
#include "abstractpiecetablechange.h"

namespace KPieceTable
{

/** class
  *@author Friedrich W. H. Kossebau
  */

class InsertPieceTableChange : public AbstractPieceTableChange
{
  public:
    InsertPieceTableChange( int insertOffset, int insertLength, int storageOffset );
    virtual ~InsertPieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual bool merge( const AbstractPieceTableChange *other );
    virtual KHE::KSection apply( PieceTable *pieceTable );
    virtual KHE::KSection revert( PieceTable *pieceTable );
    virtual KHE::ReplacementScope replacement() const;

  protected:
    int mInsertOffset;
    int mInsertLength;
    int mStorageOffset;
};

inline InsertPieceTableChange::InsertPieceTableChange( int insertOffset, int insertLength, int storageOffset )
 : mInsertOffset( insertOffset ), mInsertLength( insertLength ), mStorageOffset( storageOffset )
{}


}

#endif
