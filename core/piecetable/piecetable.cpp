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

#include "piecetable.h"

// Qt
#include <QtCore/QMutableLinkedListIterator>


namespace KPieceTable
{

PieceTable::PieceTable( Size size )
{
    init( size );
}

void PieceTable::init( Size size )
{
    mList.clear();
    if( size > 0 )
        mList.append( Piece(0,size,Piece::OriginalStorage) );

    mSize = size;
}


bool PieceTable::getStorageData( int* storageId, Address* storageOffset, Address dataOffset ) const
{
    bool result = false;

    // TODO: use width or offset from current and next?
    AddressRange dataRange( 0, -1 );
    foreach( const Piece& piece, mList )
    {
        dataRange.setEndByWidth( piece.width() );

        if( dataRange.includes(dataOffset) )
        {
            *storageId = piece.storageId();
// kDebug() <<piece.start()<<"+"<<dataRange.localIndex( dataOffset );
            *storageOffset = piece.start() + dataRange.localIndex( dataOffset );
            result = true;
            break;
        }
        dataRange.setStart( dataRange.nextBehindEnd() );
    }

    return result;
}

// TODO: optimize search from behind if dataOffset is large (perhaps by counting total size
// TODO: combine sucsequenting inserts, also on other changes if possible link neighbors
void PieceTable::insert( Address insertDataOffset, Size insertLength, Address storageOffset )
{
    const int storageId = Piece::ChangeStorage;
    QMutableLinkedListIterator<Piece> it( mList );

    const Piece insertPiece( storageOffset, insertLength, storageId );

    // TODO: use width or offset from current and next?
    AddressRange dataRange( 0, -1 );
    while( it.hasNext() )
    {
        Piece& piece = it.peekNext();
        dataRange.setEndByWidth( piece.width() );

        // piece starts at offset?
        if( dataRange.start() == insertDataOffset )
        {
            it.insert( insertPiece );
            break;
        }
        if( dataRange.nextBehindEnd() == insertDataOffset )
        {
            if( piece.append(insertPiece) )
                break;
        }
        it.next();
        if( dataRange.includes(insertDataOffset) )
        {
            const Piece secondPiece = piece.splitAtLocal( dataRange.localIndex(insertDataOffset) );
            it.insert( insertPiece );
            it.insert( secondPiece );
            break;
        }

        dataRange.setStart( dataRange.nextBehindEnd() );
    }
    if( !it.hasNext() && (dataRange.start() == insertDataOffset) )
        it.insert( insertPiece );

    mSize += insertLength;
}

void PieceTable::insert( Address insertDataOffset, const PieceList& insertPieceList )
{
    if( insertPieceList.isEmpty() )
        return;

    bool isInserted = false;
    QMutableLinkedListIterator<Piece> it( mList );

    // TODO: use width or offset from current and next?
    AddressRange dataRange( 0, -1 );
    while( it.hasNext() )
    {
        Piece& piece = it.peekNext();
        dataRange.setEndByWidth( piece.width() );

        // piece starts at offset?
        if( dataRange.start() == insertDataOffset )
        {
            int i = 0;
            if( it.hasPrevious() )
            {
                Piece& previousPiece = it.peekPrevious();
                if( previousPiece.append(insertPieceList.at(0)) )
                {
                    if( insertPieceList.size() == 1 )
                    {
                        if( previousPiece.append(piece) )
                        {
                            it.next();
                            it.remove();
                        }
                        isInserted = true;
                        break;
                    }
                    ++i;
                }
            }

            const int lastIndex = insertPieceList.size()-1;
            for( ; i<lastIndex; ++i )
                it.insert( insertPieceList.at(i) );
            const Piece& lastInsertPiece = insertPieceList.at( lastIndex );
            if( ! piece.prepend(lastInsertPiece) )
                it.insert( lastInsertPiece );
            isInserted = true;
            break;
        }
        it.next();
        if( dataRange.includes(insertDataOffset) )
        {
            const Piece secondPiece = piece.splitAtLocal( dataRange.localIndex(insertDataOffset) );
            for( int i=0; i<insertPieceList.size(); ++i )
                it.insert( insertPieceList.at(i) );
            it.insert( secondPiece );
            isInserted = true;
            break;
        }

        dataRange.setStart( dataRange.nextBehindEnd() );
    }
    if( ! isInserted && (dataRange.start() == insertDataOffset) )
    {
        int i = 0;
        if( it.hasPrevious() )
        {
            Piece& previousPiece = it.peekPrevious();
            if( previousPiece.append(insertPieceList.at(0)) )
                ++i;
        }
        for( ; i<insertPieceList.size(); ++i )
            it.insert( insertPieceList.at(i) );
    }

    mSize += insertPieceList.totalLength();
}

// TODO: make algorithm simpler
PieceList PieceTable::remove( const AddressRange& removeRange )
{
    PieceList removedPieceList;

    AddressRange dataRange( 0, -1 );

    QLinkedList<Piece>::Iterator it = mList.begin();
    while( it != mList.end() )
    {
        Piece* piece = &*it;
        dataRange.setEndByWidth( piece->width() );

        if( dataRange.includes(removeRange.start()) )
        {
            QLinkedList<Piece>::Iterator firstRemoved = it;
            const Address firstDataRangeStart = dataRange.start();
// int sections = 1;

            if( dataRange.includesInside(removeRange) )
            {
                const AddressRange localRange = dataRange.localRange( removeRange );
                const Piece removedPiece = piece->subPiece( localRange );
                const Piece secondPiece = piece->removeLocal( localRange );

                mList.insert( ++it, secondPiece );
                removedPieceList.append( removedPiece );
// kDebug() << "removed intern";
                break;
            }
            do
            {
                if( dataRange.includes(removeRange.end()) )
                {
                    QLinkedList<Piece>::Iterator lastRemoved = it;
// kDebug() << removeRange.start() << removeRange.end() << firstDataRangeStart << dataRange.end();
                    // cut from first section if not all
                    bool onlyCompletePiecesRemoved = true;
                    if( firstDataRangeStart < removeRange.start() )
                    {
                        const Address newLocalEnd = removeRange.start() - firstDataRangeStart - 1;
                        const Piece removedPiece = (*firstRemoved).removeEndBehindLocal( newLocalEnd );
                        removedPieceList.append( removedPiece );

                        ++firstRemoved;
                        onlyCompletePiecesRemoved = false;
// kDebug() << "end of first removed"<<piece->start()<<piece->end()<<"->"<<removedPiece.start()<<removedPiece.end();
// --sections;
                    }

                    Piece removedPartialPieceFromLast;
                    // cut from last section if not all
                    if( removeRange.end() < dataRange.end() )
                    {
                        const Address newLocalStart =  dataRange.localIndex( removeRange.end() ) + 1;
                        removedPartialPieceFromLast = piece->removeStartBeforeLocal( newLocalStart );

                        onlyCompletePiecesRemoved = false;
// kDebug() << "start of last removed"<<piece->start()<<piece->end()<<"->"<<removedPartialPieceFromLast.start()<<removedPartialPieceFromLast.end();
// --sections;
                    }
                    else
                    {
                        ++lastRemoved;
                    }

                    for( QLinkedList<Piece>::Iterator it = firstRemoved; it!=lastRemoved; ++it )
                        removedPieceList.append( *it );
                    if( removedPartialPieceFromLast.isValid() )
                        removedPieceList.append( removedPartialPieceFromLast );

                    if( onlyCompletePiecesRemoved )
                    {
                        if( firstRemoved != mList.begin() && lastRemoved != mList.end() )
                        {
                            QLinkedList<Piece>::Iterator beforeFirstRemoved = firstRemoved - 1;
                            if( (*beforeFirstRemoved).append(*lastRemoved) )
                                ++lastRemoved;
                        }
                    }

                    mList.erase( firstRemoved, lastRemoved );
// kDebug() << "removed "<<sections;
                    break;
                }
                dataRange.setStart( dataRange.nextBehindEnd() );
                ++it;
// ++sections;
                // removeRange is longer than content TODO: just quit or at least remove till the end?
                if( it == mList.end() )
                    break;
                piece = &*it;
                dataRange.setEndByWidth( piece->width() );
            }
            while( it != mList.end() );
            break;
        }

        dataRange.setStart( dataRange.nextBehindEnd() );
        ++it;
    }

    mSize -= removeRange.width();

// kDebug()<<"end:"<<asStringList(mList);
    return removedPieceList;
}

PieceList PieceTable::replace( const AddressRange& removeRange, Size insertLength, Address storageOffset )
{
    PieceList removedPieceList = remove( removeRange );
    insert( removeRange.start(), insertLength, storageOffset );
    return removedPieceList;
}
void PieceTable::replace( const AddressRange& removeRange, const PieceList& insertPieceList )
{
    remove( removeRange );
    insert( removeRange.start(), insertPieceList );
}

void PieceTable::swap( Address firstStart, const AddressRange& secondRange )
{
    AddressRange dataRange( 0, -1 );

    QLinkedList<Piece>::Iterator it = mList.begin();
    while( it != mList.end() )
    {
        Piece* piece = &*it;
        dataRange.setEndByWidth( piece->width() );

        if( dataRange.includes(firstStart) )
        {
            // piece does not start at offset?
            if( dataRange.start() < firstStart )
            {
                // well, split and insert second piece, even if we move it later, just make it work for now
                const Piece secondPiece = piece->splitAtLocal( dataRange.localIndex(firstStart) );
                it = mList.insert( ++it, secondPiece );
                piece = &*it;
                dataRange.setStart( firstStart );
            }
            QLinkedList<Piece>::Iterator firstIt = it;

            do
            {
                if( dataRange.includes(secondRange.start()) )
                {
                    // piece does not start at source?
                    if( dataRange.start() < secondRange.start() )
                    {
                        // well, split and insert second piece, even if we move it later, just make it work for now
                        const Piece secondPiece = piece->splitAtLocal( dataRange.localIndex(secondRange.start()) );
                        it = mList.insert( ++it, secondPiece );
                        piece = &*it;
                        dataRange.setStart( secondRange.start() );
                    }
                    QLinkedList<Piece>::Iterator firstSecondIt = it;

                    do
                    {
                        if( dataRange.includes(secondRange.end()) )
                        {
                            // piece does not start at source?
                            if( secondRange.end() < dataRange.end() )
                            {
                                // well, split and insert second piece, even if we move it later, just make it work for now
                                const Piece secondPiece = piece->splitAtLocal( dataRange.localIndex(secondRange.nextBehindEnd()) );
                                it = mList.insert( ++it, secondPiece );
                            }
                            else
                                ++it;
                            QLinkedList<Piece>::Iterator behindLastSecondIt = it;

                            for( it=firstSecondIt; it!=behindLastSecondIt; ++it )
                            {
                                firstIt = mList.insert( firstIt, *it ) + 1;
                            }
                            mList.erase( firstSecondIt, behindLastSecondIt );

                            // done, move out of here
                            it = mList.end();
                            break;
                        }
                        dataRange.setStart( dataRange.nextBehindEnd() );
                        ++it;

                        // removeRange is longer than content TODO: just quit or at least remove till the end?
                        if( it == mList.end() )
                            break;
                        piece = &*it;
                        dataRange.setEndByWidth( piece->width() );
                    }
                    while( it != mList.end() );
                }
                else
                {
                    dataRange.setStart( dataRange.nextBehindEnd() );
                    ++it;

                    // removeRange is longer than content TODO: just quit or at least remove till the end?
                    if( it == mList.end() )
                        break;
                    piece = &*it;
                    dataRange.setEndByWidth( piece->width() );
                }
            }
            while( it != mList.end() );
            break;
        }

        dataRange.setStart( dataRange.nextBehindEnd() );
        ++it;
    }
}

Piece PieceTable::replaceOne( Address dataOffset, Address storageOffset, int storageId )
{
    int replacedStorageId = Piece::OriginalStorage;
    Address replacedStorageOffset = -1;

    QMutableLinkedListIterator<Piece> it( mList );

    AddressRange dataRange( 0, -1 );
    while( it.hasNext() )
    {
        Piece* piece = &it.peekNext();
        dataRange.setEndByWidth( piece->width() );
        if( dataRange.includes(dataOffset) )
        {
            replacedStorageId = piece->storageId();

            const Piece replacePiece( storageOffset, 1, storageId );
            // piece starts at offset?
            if( dataRange.start() == dataOffset )
            {
                replacedStorageOffset = piece->start();
                if( dataRange.width() == 1 )
                {
                    piece->set( storageOffset, storageOffset );
                    piece->setStorageId( storageId );
                }
                else
                {
                    it.insert( replacePiece );
                    piece->moveStartBy( 1 );
                }
            }
            else if( dataRange.end() == dataOffset )
            {
                replacedStorageOffset = piece->end();
                piece->moveEndBy( -1 );
                it.next();
                it.insert( replacePiece );
            }
            else
            {
                const Address localIndex = dataRange.localIndex( dataOffset );
                replacedStorageOffset = piece->start() + localIndex;

                const Piece secondPiece = piece->removeLocal( AddressRange::fromWidth(localIndex,1) );
                it.next();
                it.insert( replacePiece );
                it.insert( secondPiece );
            }
            break;
        }
        it.next();
        dataRange.setStart( dataRange.nextBehindEnd() );
    }
    return Piece( replacedStorageOffset, 1, replacedStorageId );
}

}
