/*
    This file is part of the Okteta Core library, made within the KDE community.

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

#include "swaprangespiecetablechange.h"

// lib
#include "piecetable.h"
//
#include <arraychangemetrics.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int SwapRangesPieceTableChange::type() const { return SwapRangesId; }

QString SwapRangesPieceTableChange::description() const
{
    return i18nc( "name of the change", "Swap Ranges" );
}

AddressRange SwapRangesPieceTableChange::apply( PieceTable* pieceTable ) const
{
    pieceTable->swap( mFirstStart, mSecondRange );

    return AddressRange( mFirstStart, mSecondRange.end() );
}

AddressRange SwapRangesPieceTableChange::revert( PieceTable* pieceTable ) const
{
    pieceTable->swap( mFirstStart, AddressRange(mFirstStart+mSecondRange.width(),mSecondRange.end()) );

    return AddressRange( mFirstStart, mSecondRange.end() );
}

ArrayChangeMetrics SwapRangesPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asSwapping( mFirstStart, mSecondRange.start(), mSecondRange.width() );
}

SwapRangesPieceTableChange::~SwapRangesPieceTableChange() {}

}
