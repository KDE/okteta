/***************************************************************************
                          swaprangespiecetablechange.cpp  -  description
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

#include "swaprangespiecetablechange.h"

// lib
#include "piecetable.h"
#include <ksection.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int SwapRangesPieceTableChange::type() const { return SwapRangesId; }

QString SwapRangesPieceTableChange::description() const
{
    return i18n( "Swap Ranges" );
}

KHE::KSection SwapRangesPieceTableChange::apply( PieceTable *pieceTable )
{
    pieceTable->swap( mFirstStart, mSecondSection );

    return KHE::KSection( mFirstStart, mSecondSection.end() );
}

KHE::KSection SwapRangesPieceTableChange::revert( PieceTable *pieceTable )
{
    pieceTable->swap( mFirstStart, KHE::KSection(mFirstStart+mSecondSection.width(),mSecondSection.end()) );

    return KHE::KSection( mFirstStart, mSecondSection.end() );
}

SwapRangesPieceTableChange::~SwapRangesPieceTableChange() {}

}
