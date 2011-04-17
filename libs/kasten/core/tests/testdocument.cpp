/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "testdocument.h"


namespace Kasten
{

TestDocument::TestDocument()
  : mLocalSyncState( LocalInSync )
{
}
TestDocument::TestDocument( const QByteArray &data )
  : mData( data ),
    mLocalSyncState( LocalInSync )
{
}

QString TestDocument::mimeType() const { return QString::fromLatin1("TestDocument"); }
QString TestDocument::typeName() const { return QString::fromLatin1("Test Document"); }
QString TestDocument::title() const { return mTitle; }
LocalSyncState TestDocument::localSyncState() const { return mLocalSyncState; }
const QByteArray* TestDocument::data() const { return &mData; }

void TestDocument::setData( const QByteArray& data )
{
    const LocalSyncState oldLocalSyncState = mLocalSyncState;

    mData = data;

    mLocalSyncState = LocalHasChanges;
    if( oldLocalSyncState != mLocalSyncState )
        emit localSyncStateChanged( mLocalSyncState );
}

void TestDocument::setTitle( const QString &title )
{
    if( mTitle != title )
    {
        mTitle = title;
        emit titleChanged( title );
    }
 }
void TestDocument::setLocalSyncState( LocalSyncState localSyncState )
{
    if( mLocalSyncState != localSyncState )
    {
        mLocalSyncState = localSyncState;
        emit localSyncStateChanged( localSyncState );
    }
}

TestDocument::~TestDocument()
{
}

}
