/***************************************************************************
                          kfilebytearraymodel.cpp  -  description
                             -------------------
    begin                : Mit Jun 02 2003
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


#include "kfilebytearraymodel.h"

// lib
#include "kfilebytearraymodel_p.h"
// C
#include <stdlib.h>


namespace KHECore {

KFileByteArrayModel::KFileByteArrayModel( int pageNumber, int pageSize )
 : d( new KFileByteArrayModelPrivate(pageNumber,pageSize) )
{
}

int KFileByteArrayModel::size()        const { return d->size(); }
bool KFileByteArrayModel::isReadOnly() const { return d->isReadOnly(); }
bool KFileByteArrayModel::isModified() const { return false; }
bool KFileByteArrayModel::isOpen()     const { return d->isOpen(); }

void KFileByteArrayModel::setReadOnly( bool readOnly ) { d->setReadOnly( readOnly ); }
void KFileByteArrayModel::setModified( bool )  {}
void KFileByteArrayModel::setDatum( unsigned int, const char )  {}
char KFileByteArrayModel::datum( unsigned int datumOffset ) const { return d->datum( datumOffset ); }

int KFileByteArrayModel::insert( int /*Pos*/, const char*, int /*Length*/ ) {  return 0; }
int KFileByteArrayModel::remove( const KSection &/*Section*/ ) {  return 0; }
unsigned int KFileByteArrayModel::replace( const KSection &/*Section*/, const char*, unsigned int /*Length*/ ) {  return 0; }
int KFileByteArrayModel::fill( char /*FillChar*/, unsigned int /*Pos*/, int /*Length*/ ) {  return 0; }
bool KFileByteArrayModel::swap( int /*DestPos*/, const KSection &/*SourceSection*/ ) { return false; }

bool KFileByteArrayModel::open( const QString &fileName ) { return d->open( fileName ); }
bool KFileByteArrayModel::close() { return d->close(); }

KFileByteArrayModel::~KFileByteArrayModel()
{
    delete d;
}

}
