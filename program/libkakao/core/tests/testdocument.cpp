/***************************************************************************
                          testdocument.cpp  -  description
                             -------------------
    begin                : Thu Oct 25 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "testdocument.h"

// Qt
#include <QtCore/QLatin1String>

TestDocument::TestDocument()
{
}
TestDocument::TestDocument( const QByteArray &data )
 : mData( data )
{
}

QString TestDocument::mimeType() const { return QLatin1String("TestDocument"); }
QString TestDocument::typeName() const { return QLatin1String("Test Document"); }
QString TestDocument::title() const { return mTitle; }
TestDocument::SynchronizationStates TestDocument::synchronizationStates() const { return mSynchronizationStates; }
const QByteArray* TestDocument::data() const { return &mData; }
void TestDocument::setData( const QByteArray &data )
{
    mData = data;
    mSynchronizationStates |= LocalHasChanges;
    emit modified( mSynchronizationStates );
}

void TestDocument::setTitle( const QString &title )
{
    if( mTitle != title )
    {
        mTitle = title;
        emit titleChanged( title );
    }
 }
void TestDocument::setSynchronizationStates( SynchronizationStates synchronizationStates )
{
    if( mSynchronizationStates != synchronizationStates )
    {
        mSynchronizationStates = synchronizationStates;
        emit modified( synchronizationStates );
    }
}

TestDocument::~TestDocument()
{
}
