/***************************************************************************
                          abstractpiecetablechange.h  -  description
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


#ifndef KPIECETABLE_ABSTRACTPIECETABLECHANGE_H
#define KPIECETABLE_ABSTRACTPIECETABLECHANGE_H


class QString;

namespace KHE
{
class KSection;
class ReplacementScope;
}

namespace KPieceTable
{

class PieceTable;

/** class
  *@author Friedrich W. H. Kossebau
  */

class AbstractPieceTableChange
{
  public:
    enum TypeId
    {
        InsertId,
        RemoveId,
        ReplaceId,
        SwapRangesId,
        FillId,
        ReplaceOneId
    };

  public:
    virtual ~AbstractPieceTableChange();

  public: // API to be implemented
    virtual int type() const = 0;

    virtual QString description() const = 0;

    /// returns true if successful, false otherwise. Default returns false.
    virtual bool merge( const AbstractPieceTableChange *other );

    virtual KHE::KSection apply( PieceTable *pieceTable ) = 0;
    virtual KHE::KSection revert( PieceTable *pieceTable ) = 0;

    virtual KHE::ReplacementScope replacement() const; // TODO: this should be any changes, like swap and co.
};

inline AbstractPieceTableChange::~AbstractPieceTableChange() {}

}

#endif
