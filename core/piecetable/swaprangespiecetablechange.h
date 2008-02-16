/***************************************************************************
                          swaprangespiecetablechange.h  -  description
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


#ifndef KPIECETABLE_SWAPRANGESPIECETABLECHANGE_H
#define KPIECETABLE_SWAPRANGESPIECETABLECHANGE_H


// lib
#include "abstractpiecetablechange.h"
// common
#include <ksection.h>

namespace KPieceTable
{

/** class
  *@author Friedrich W. H. Kossebau
  */

class SwapRangesPieceTableChange : public AbstractPieceTableChange
{
  public:
    SwapRangesPieceTableChange( int firstStart, const KHE::KSection &secondSection );
    virtual ~SwapRangesPieceTableChange();

  public: // AbstractPieceTableChange API
    virtual int type() const;
    virtual QString description() const;
    virtual KHE::KSection apply( PieceTable *pieceTable );
    virtual KHE::KSection revert( PieceTable *pieceTable );

  protected:
    int mFirstStart;
    KHE::KSection mSecondSection;
};

inline SwapRangesPieceTableChange::SwapRangesPieceTableChange( int firstStart, const KHE::KSection &secondSection )
 : mFirstStart( firstStart ), mSecondSection( secondSection )
{}

}

#endif
