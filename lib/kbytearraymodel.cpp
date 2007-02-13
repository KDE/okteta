/***************************************************************************
                          kbytearraymodel.cpp  -  description
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


// lib specific
#include "kbytearraymodel_p.h"
#include "kbytearraymodel.h"


namespace KHECore {

KByteArrayModel::KByteArrayModel( char *data, unsigned int size, int rawSize, bool keepMemory )
 : d( new KByteArrayModelPrivate(this,data,size,rawSize,keepMemory) )
{}

KByteArrayModel::KByteArrayModel( const char *data, unsigned int size )
 : d( new KByteArrayModelPrivate(this,data,size) )
{}

KByteArrayModel::KByteArrayModel( int size, int maxSize )
 : d( new KByteArrayModelPrivate(this,size,maxSize) )
{}


char KByteArrayModel::datum( unsigned int offset ) const { return d->datum(offset); }
int KByteArrayModel::size()                        const { return d->size(); }

bool KByteArrayModel::isReadOnly()   const { return d->isReadOnly(); }
bool KByteArrayModel::isModified()   const { return d->isModified(); }

void KByteArrayModel::setReadOnly( bool readOnly )       { d->setReadOnly( readOnly ); }
void KByteArrayModel::setModified( bool modified )       { d->setModified( modified ); }
void KByteArrayModel::setMaxSize( int maxSize )          { d->setMaxSize( maxSize ); }
void KByteArrayModel::setKeepsMemory( bool keepsMemory ) { d->setKeepsMemory( keepsMemory ); }
void KByteArrayModel::setAutoDelete( bool autoDelete )   { d->setAutoDelete( autoDelete ); }

char *KByteArrayModel::data()       const { return d->data(); }
int KByteArrayModel::maxSize()      const { return d->maxSize(); }
bool KByteArrayModel::keepsMemory() const { return d->keepsMemory(); }
bool KByteArrayModel::autoDelete()  const { return d->autoDelete(); }

void KByteArrayModel::signalContentsChanged( int i1, int i2 ) { emit contentsChanged(i1,i2); }


void KByteArrayModel::setDatum( unsigned int offset, const char datum )
{
    d->setDatum( offset, datum );
}

int KByteArrayModel::insert( int at, const char *data, int length )
{
    return d->insert( at, data, length );
}

int KByteArrayModel::remove( const KSection &section )
{
    return d->remove( section );
}

unsigned int KByteArrayModel::replace( const KSection &before, const char *after, unsigned int afterLength )
{
    return d->replace( before, after, afterLength );
}

int KByteArrayModel::move( int to, const KSection &fromSection )
{
    return d->move( to, fromSection );
}

int KByteArrayModel::fill( const char fillChar, unsigned int from, int length )
{
    return d->fill( fillChar, from, length );
}

int KByteArrayModel::indexOf( const char *searchString, int length, int from ) const
{
    return d->indexOf( searchString, length, from );
}

int KByteArrayModel::lastIndexOf( const char *searchString, int length, int from ) const
{
    return d->lastIndexOf( searchString, length, from );
}

KByteArrayModel::~KByteArrayModel()
{
    delete d;
}

}
