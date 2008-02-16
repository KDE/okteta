/***************************************************************************
                          testpiecetablechange.h  -  description
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


#ifndef KPIECETABLE_TESTPIECETABLECHANGE_H
#define KPIECETABLE_TESTPIECETABLECHANGE_H


// lib
#include <piecetable/abstractpiecetablechange.h>
#include <piecetable/piece.h>
// Qt
#include <QtCore/QString>

namespace KPieceTable
{

class TestPieceTableChange : public AbstractPieceTableChange
{
  public:
    TestPieceTableChange( int typeId = -1, const QString &description = QString(),
                          int position = 0, int storagePosition = 0, int storageId = Piece::ChangeStorage,
                          int replacedStoragePosition = 0, int replacedStorageId = Piece::ChangeStorage );
    virtual ~TestPieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual bool merge( const AbstractPieceTableChange *other );
    virtual KHE::KSection apply( PieceTable *pieceTable );
    virtual KHE::KSection revert( PieceTable *pieceTable );

  protected:
    int mTypeId;
    QString mDescription;
    int mPosition;
    int mStoragePosition;
    int mStorageId;

    int mReplacedStoragePosition;
    int mReplacedStorageId;
};

inline TestPieceTableChange::TestPieceTableChange( int typeId, const QString &description,
                                                   int position, int storagePosition, int storageId,
                                                   int replacedStoragePosition, int replacedStorageId )
 : mTypeId( typeId ), mDescription( description),
   mPosition( position ), mStoragePosition( storagePosition ), mStorageId( storageId ),
   mReplacedStoragePosition( replacedStoragePosition ), mReplacedStorageId( replacedStorageId )
{}

}

#endif
