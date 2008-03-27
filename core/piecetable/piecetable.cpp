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

#include "piecetable.h"

// Qt
#include <QtCore/QMutableLinkedListIterator>

namespace KPieceTable
{

PieceTable::PieceTable( int size )
{
    init( size );
}

void PieceTable::init( int size )
{
    mList.clear();
    if( size > 0 )
        mList.append( Piece(0,size,Piece::OriginalStorage) );

    mSize = size;
}


bool PieceTable::getStorageData( int *storageId, int *storageOffset, int dataOffset ) const
{
    bool result = false;
    // TODO: use width or offset from current and next?
    KHE::KSection dataSection( 0, -1 );
    foreach( Piece piece, mList )
    {
        dataSection.setEndByWidth( piece.width() );

        if( dataSection.includes(dataOffset) )
        {
            *storageId = piece.storageId();
// kDebug() <<piece.start()<<"+"<<dataSection.localIndex( dataOffset );
            *storageOffset = piece.start() + dataSection.localIndex( dataOffset );
            result = true;
            break;
        }
        dataSection.setStart( dataSection.behindEnd() );
    }
    return result;
}

// TODO: optimise search from behind if dataOffset is large (perhaps by counting total size
// TODO: combine sucsequenting inserts, also on other changes if possible link neighbors
void PieceTable::insert( int insertDataOffset, int insertLength, int storageOffset )
{
    const int storageId = Piece::ChangeStorage;
    QMutableLinkedListIterator<Piece> it( mList );

    const Piece insertPiece( storageOffset, insertLength, storageId );

    // TODO: use width or offset from current and next?
    KHE::KSection dataSection( 0, -1 );
    while( it.hasNext() )
    {
        Piece &piece = it.peekNext();
        dataSection.setEndByWidth( piece.width() );

        // piece starts at offset?
        if( dataSection.start() == insertDataOffset )
        {
            it.insert( insertPiece );
            break;
        }
        if( dataSection.behindEnd() == insertDataOffset )
        {
            if( piece.append(insertPiece) )
                break;
        }
        it.next();
        if( dataSection.includes(insertDataOffset) )
        {
            const Piece secondPiece = piece.splitAtLocal( dataSection.localIndex(insertDataOffset) );
            it.insert( insertPiece );
            it.insert( secondPiece );
            break;
        }

        dataSection.setStart( dataSection.behindEnd() );
    }
    if( !it.hasNext() && (dataSection.start() == insertDataOffset) )
        it.insert( insertPiece );

    mSize += insertLength;
}

void PieceTable::insert( int insertDataOffset, const PieceList &insertPieceList )
{
    if( insertPieceList.isEmpty() )
        return;

    bool isInserted = false;
    QMutableLinkedListIterator<Piece> it( mList );

    // TODO: use width or offset from current and next?
    KHE::KSection dataSection( 0, -1 );
    while( it.hasNext() )
    {
        Piece &piece = it.peekNext();
        dataSection.setEndByWidth( piece.width() );

        // piece starts at offset?
        if( dataSection.start() == insertDataOffset )
        {
            int i = 0;
            if( it.hasPrevious() )
            {
                Piece &previousPiece = it.peekPrevious();
                if( previousPiece.append(insertPieceList.at(0)) )
                {
                    if( insertPieceList.size() == 1 )
                    {
                        if( previousPiece.append(piece) )
                        {
                            it.next();
                            it.remove();
                            isInserted = true;
                            break;
                        }
                    }
                    ++i;
                }
            }

            const int lastIndex = insertPieceList.size()-1;
            for( ; i<lastIndex; ++i )
                it.insert( insertPieceList.at(i) );
            const Piece &lastInsertPiece = insertPieceList.at( lastIndex );
            if( !piece.prepend(lastInsertPiece) )
                it.insert( lastInsertPiece );
            isInserted = true;
            break;
        }
        it.next();
        if( dataSection.includes(insertDataOffset) )
        {
            const Piece secondPiece = piece.splitAtLocal( dataSection.localIndex(insertDataOffset) );
            for( int i=0; i<insertPieceList.size(); ++i )
                it.insert( insertPieceList.at(i) );
            it.insert( secondPiece );
            isInserted = true;
            break;
        }

        dataSection.setStart( dataSection.behindEnd() );
    }
    if( !isInserted && (dataSection.start() == insertDataOffset) )
    {
        int i = 0;
        if( it.hasPrevious() )
        {
            Piece &previousPiece = it.peekPrevious();
            if( previousPiece.append(insertPieceList.at(0)) )
                ++i;
        }
        for( ; i<insertPieceList.size(); ++i )
            it.insert( insertPieceList.at(i) );
    }

    mSize += insertPieceList.totalLength();
}


PieceList PieceTable::remove( const KHE::KSection &removeSection )
{
    PieceList removedPieceList;

    KHE::KSection dataSection( 0, -1 );

    QLinkedList<Piece>::Iterator it = mList.begin();
    while( it != mList.end() )
    {
        Piece *piece = &*it;
        dataSection.setEndByWidth( piece->width() );

        if( dataSection.includes(removeSection.start()) )
        {
            QLinkedList<Piece>::Iterator firstRemoved = it;
            const int firstDataSectionStart = dataSection.start();
// int sections = 1;

            if( dataSection.includesInside(removeSection) )
            {
                const KHE::KSection localSection = dataSection.localSection( removeSection );
                const Piece removedPiece = piece->subPiece( localSection );
                const Piece secondPiece = piece->removeLocal( localSection );

                mList.insert( ++it, secondPiece );
                removedPieceList.append( removedPiece );
// kDebug() << "removed intern";
                break;
            }
            do
            {
                if( dataSection.includes(removeSection.end()) )
                {
                    QLinkedList<Piece>::Iterator lastRemoved = it;
// kDebug() << removeSection.start() << removeSection.end() << firstDataSectionStart << dataSection.end();
                    // cut from first section if not all
                    bool onlyCompletePiecesRemoved = true;
                    if( firstDataSectionStart < removeSection.start() )
                    {
                        const int newLocalEnd = removeSection.start() - firstDataSectionStart - 1;
                        const Piece removedPiece = (*firstRemoved).removeEndBehindLocal( newLocalEnd );
                        removedPieceList.append( removedPiece );

                        ++firstRemoved;
                        onlyCompletePiecesRemoved = false;
// kDebug() << "end of first removed"<<piece->start()<<piece->end()<<"->"<<removedPiece.start()<<removedPiece.end();
// --sections;
                    }
                    // cut from last section if not all
                    if( removeSection.end() < dataSection.end() )
                    {
                        const int newLocalStart =  dataSection.localIndex( removeSection.end() ) + 1;
                        const Piece removedPiece = piece->removeStartBeforeLocal( newLocalStart );
                        removedPieceList.append( removedPiece );

                        --lastRemoved;
                        onlyCompletePiecesRemoved = false;
// kDebug() << "start of last removed"<<piece->start()<<piece->end()<<"->"<<removedPiece.start()<<removedPiece.end();
// --sections;
                    }
                    ++lastRemoved;

                    for( QLinkedList<Piece>::Iterator it = firstRemoved; it!=lastRemoved; ++it )
                        removedPieceList.append( *it );

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
                dataSection.setStart( dataSection.behindEnd() );
                ++it;
// ++sections;
                // removeSection is longer than content TODO: just quit or at least remove till the end?
                if( it == mList.end() )
                    break;
                piece = &*it;
                dataSection.setEndByWidth( piece->width() );
            }
            while( it != mList.end() );
            break;
        }

        dataSection.setStart( dataSection.behindEnd() );
        ++it;
    }

    mSize -= removeSection.width();

// kDebug()<<"end:"<<asStringList(mList);
    return removedPieceList;
}

PieceList PieceTable::replace( const KHE::KSection &removeSection, int insertLength, int storageOffset )
{
    PieceList removedPieceList = remove( removeSection );
    insert( removeSection.start(), insertLength, storageOffset );
    return removedPieceList;
}
void PieceTable::replace( const KHE::KSection &removeSection, const PieceList &insertPieceList )
{
    remove( removeSection );
    insert( removeSection.start(), insertPieceList );
}

void PieceTable::swap( int firstStart, const KHE::KSection &secondSection )
{
    KHE::KSection dataSection( 0, -1 );

    QLinkedList<Piece>::Iterator it = mList.begin();
    while( it != mList.end() )
    {
        Piece *piece = &*it;
        dataSection.setEndByWidth( piece->width() );

        if( dataSection.includes(firstStart) )
        {
            // piece does not start at offset?
            if( dataSection.start() < firstStart )
            {
                // well, split and insert second piece, even if we move it later, just make it work for now
                const Piece secondPiece = piece->splitAtLocal( dataSection.localIndex(firstStart) );
                it = mList.insert( ++it, secondPiece );
                piece = &*it;
                dataSection.setStart( firstStart );
            }
            QLinkedList<Piece>::Iterator firstIt = it;

            do
            {
                if( dataSection.includes(secondSection.start()) )
                {
                    // piece does not start at source?
                    if( dataSection.start() < secondSection.start() )
                    {
                        // well, split and insert second piece, even if we move it later, just make it work for now
                        const Piece secondPiece = piece->splitAtLocal( dataSection.localIndex(secondSection.start()) );
                        it = mList.insert( ++it, secondPiece );
                        piece = &*it;
                        dataSection.setStart( secondSection.start() );
                    }
                    QLinkedList<Piece>::Iterator firstSecondIt = it;

                    do
                    {
                        if( dataSection.includes(secondSection.end()) )
                        {
                            // piece does not start at source?
                            if( secondSection.end() < dataSection.end() )
                            {
                                // well, split and insert second piece, even if we move it later, just make it work for now
                                const Piece secondPiece = piece->splitAtLocal( dataSection.localIndex(secondSection.end()+1) );
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
                        dataSection.setStart( dataSection.behindEnd() );
                        ++it;

                        // removeSection is longer than content TODO: just quit or at least remove till the end?
                        if( it == mList.end() )
                            break;
                        piece = &*it;
                        dataSection.setEndByWidth( piece->width() );
                    }
                    while( it != mList.end() );
                }
                else
                {
                    dataSection.setStart( dataSection.behindEnd() );
                    ++it;

                    // removeSection is longer than content TODO: just quit or at least remove till the end?
                    if( it == mList.end() )
                        break;
                    piece = &*it;
                    dataSection.setEndByWidth( piece->width() );
                }
            }
            while( it != mList.end() );
            break;
        }

        dataSection.setStart( dataSection.behindEnd() );
        ++it;
    }
}

Piece PieceTable::replaceOne( int dataOffset, int storageOffset, int storageId )
{
    int replacedStorageId = Piece::OriginalStorage;
    int replacedStorageOffset = -1;

    QMutableLinkedListIterator<Piece> it( mList );

    KHE::KSection dataSection( 0, -1 );
    while( it.hasNext() )
    {
        Piece *piece = &it.peekNext();
        dataSection.setEndByWidth( piece->width() );
        if( dataSection.includes(dataOffset) )
        {
            replacedStorageId = piece->storageId();

            const Piece replacePiece( storageOffset, 1, storageId );
            // piece starts at offset?
            if( dataSection.start() == dataOffset )
            {
                replacedStorageOffset = piece->start();
                if( dataSection.width() == 1 )
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
            else if( dataSection.end() == dataOffset )
            {
                replacedStorageOffset = piece->end();
                piece->moveEndBy( -1 );
                it.next();
                it.insert( replacePiece );
            }
            else
            {
                const int localIndex = dataSection.localIndex( dataOffset );
                replacedStorageOffset = piece->start() + localIndex;

                const Piece secondPiece = piece->removeLocal( KHE::KSection::fromWidth(localIndex,1) );
                it.next();
                it.insert( replacePiece );
                it.insert( secondPiece );
            }
            break;
        }
        it.next();
        dataSection.setStart( dataSection.behindEnd() );
    }
    return Piece( replacedStorageOffset, 1, replacedStorageId );
}

}
