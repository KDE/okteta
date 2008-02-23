/***************************************************************************
                          kpiecetablebytearraymodel.cpp  -  description
                             -------------------
    begin                : Mit Jun 03 2003
    copyright            : (C) 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "kpiecetablebytearraymodel.h"

// lib
#include "kpiecetablebytearraymodel_p.h"


namespace KHECore {


KPieceTableByteArrayModel::KPieceTableByteArrayModel( const char *data, unsigned int size,
                                                      bool careForMemory )
 : d( new Private(this,data,size,careForMemory) )
{}

KPieceTableByteArrayModel::KPieceTableByteArrayModel( unsigned int size, char fillByte )
 : d( new Private(this,size,fillByte) )
{}

// KPieceTableByteArrayModel::KPieceTableByteArrayModel( int size, int maxSize )
//  : d( new Private(this,size,maxSize) )
// {}


char KPieceTableByteArrayModel::datum( unsigned int offset ) const { return d->datum(offset); }
int KPieceTableByteArrayModel::size()                        const { return d->size(); }

bool KPieceTableByteArrayModel::isReadOnly()   const { return d->isReadOnly(); }
bool KPieceTableByteArrayModel::isModified()   const { return d->isModified(); }

void KPieceTableByteArrayModel::setReadOnly( bool readOnly )       { d->setReadOnly( readOnly ); }
void KPieceTableByteArrayModel::setModified( bool modified )       { d->setModified( modified ); }
// void KPieceTableByteArrayModel::setMaxSize( int maxSize )          { d->setMaxSize( maxSize ); }
// void KPieceTableByteArrayModel::setKeepsMemory( bool keepsMemory ) { d->setKeepsMemory( keepsMemory ); }
// void KPieceTableByteArrayModel::setAutoDelete( bool autoDelete )   { d->setAutoDelete( autoDelete ); }

void KPieceTableByteArrayModel::setData( const char *data, unsigned int size, bool careForMemory )
{ d->setData( data, size, careForMemory ); }

// char *KPieceTableByteArrayModel::data()       const { return d->data(); }
// int KPieceTableByteArrayModel::maxSize()      const { return d->maxSize(); }
// bool KPieceTableByteArrayModel::keepsMemory() const { return d->keepsMemory(); }
// bool KPieceTableByteArrayModel::autoDelete()  const { return d->autoDelete(); }

// void KPieceTableByteArrayModel::signalContentsChanged( int i1, int i2 ) { emit contentsChanged(i1,i2); }


void KPieceTableByteArrayModel::setDatum( unsigned int offset, const char datum )
{
    d->setDatum( offset, datum );
}

int KPieceTableByteArrayModel::insert( int at, const char *data, int length )
{
    return d->insert( at, data, length );
}

int KPieceTableByteArrayModel::remove( const KSection &section )
{
    return d->remove( section );
}

unsigned int KPieceTableByteArrayModel::replace( const KSection &before, const char *after, unsigned int afterLength )
{
    return d->replace( before, after, afterLength );
}

bool KPieceTableByteArrayModel::swap( int firstStart, const KSection &secondSection )
{
    return d->swap( firstStart, secondSection );
}

int KPieceTableByteArrayModel::fill( const char fillChar, unsigned int from, int length )
{
    return d->fill( fillChar, from, length );
}

// int KPieceTableByteArrayModel::indexOf( const char *searchString, int length, int from ) const
// {
//     return d->indexOf( searchString, length, from );
// }
// 
// int KPieceTableByteArrayModel::lastIndexOf( const char *searchString, int length, int from ) const
// {
//     return d->lastIndexOf( searchString, length, from );
// }

int KPieceTableByteArrayModel::versionIndex() const { return d->versionIndex(); }
int KPieceTableByteArrayModel::versionCount() const { return d->versionCount(); }
QString KPieceTableByteArrayModel::versionDescription( int versionIndex ) const { return d->versionDescription(versionIndex); }

void KPieceTableByteArrayModel::revertToVersionByIndex( int versionIndex )
{ d->revertToVersionByIndex( versionIndex ); }

void KPieceTableByteArrayModel::addBookmarks( const QList<KHECore::KBookmark> &bookmarks )
{
    d->addBookmarks( bookmarks );
}

void KPieceTableByteArrayModel::removeBookmarks( const QList<KHECore::KBookmark> &bookmarks )
{
    d->removeBookmarks( bookmarks );
}

void KPieceTableByteArrayModel::removeAllBookmarks()
{
    d->removeAllBookmarks();
}

KHECore::KBookmarkList KPieceTableByteArrayModel::bookmarkList() const
{
    return d->bookmarkList();
}

KPieceTableByteArrayModel::~KPieceTableByteArrayModel()
{
    delete d;
}

}
