/*
    This file is part of the Okteta Core library, made within the KDE community.

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

#include "filebytearraymodel.h"

// lib
#include "filebytearraymodel_p.h"
// C
#include <stdlib.h>


namespace Okteta
{

FileByteArrayModel::FileByteArrayModel( int pageNumber, int pageSize, QObject* parent )
  : AbstractByteArrayModel( parent ),
    d( new FileByteArrayModelPrivate(pageNumber,pageSize) )
{
}

Size FileByteArrayModel::size()        const { return d->size(); }
bool FileByteArrayModel::isReadOnly() const { return d->isReadOnly(); }
bool FileByteArrayModel::isModified() const { return false; }
bool FileByteArrayModel::isOpen()     const { return d->isOpen(); }

void FileByteArrayModel::setReadOnly( bool readOnly ) { d->setReadOnly( readOnly ); }
void FileByteArrayModel::setModified( bool )  {}
void FileByteArrayModel::setByte( Address, Byte )  {}
Byte FileByteArrayModel::byte( Address offset ) const { return d->byte( offset ); }

Size FileByteArrayModel::insert( Address /*Pos*/, const Byte*, int /*Length*/ ) {  return 0; }
Size FileByteArrayModel::remove( const AddressRange& /*Section*/ ) {  return 0; }
Size FileByteArrayModel::replace( const AddressRange& /*Section*/, const Byte*, int /*Length*/ ) {  return 0; }
Size FileByteArrayModel::fill( Byte /*FillChar*/, Address /*Pos*/, Size /*Length*/ ) {  return 0; }
bool FileByteArrayModel::swap( Address /*DestPos*/, const AddressRange& /*SourceSection*/ ) { return false; }

bool FileByteArrayModel::open( const QString& fileName ) { return d->open( fileName ); }
bool FileByteArrayModel::close() { return d->close(); }

FileByteArrayModel::~FileByteArrayModel()
{
    delete d;
}

}
