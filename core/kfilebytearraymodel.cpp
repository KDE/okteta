/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
