/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "bytearrayprocessdocument.h"

// 
#include "bytearrayprocessbytearrayconnector.h"
// lib
#include <kbytearraydocument.h>
// process documents
#include <processdocumentperson.h>
// Kakao core
#include <person.h>
// KDE
#include <KIcon>


ByteArrayProcessDocument::ByteArrayProcessDocument( KByteArrayDocument* byteArrayDocument )
 : mByteArrayDocument( byteArrayDocument )
{
}

QString ByteArrayProcessDocument::title() const { return mByteArrayDocument->title(); }
QString ByteArrayProcessDocument::id()    const { return mByteArrayDocument->id(); }
int ByteArrayProcessDocument::versionIndex() const { return mByteArrayDocument->versionIndex(); }
ProcessDocumentPerson ByteArrayProcessDocument::person() const
{
    const Person owner = mByteArrayDocument->owner();
    const ProcessDocumentPerson result( owner.name(), owner.faceIcon() );
    return result;
}
AbstractProcessDocumentConnector* ByteArrayProcessDocument::createConnector( const ProcessDocumentPerson& requester )
{
Q_UNUSED( requester )
    return new ByteArrayProcessByteArrayConnector( mByteArrayDocument );
}

ByteArrayProcessDocument::~ByteArrayProcessDocument()
{
    qDeleteAll( mConnectorList );
}
