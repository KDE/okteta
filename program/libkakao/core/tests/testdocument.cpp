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

TestDocument::TestDocument()
{
}

QString TestDocument::title() const { return mTitle; }
TestDocument::SynchronizationStates TestDocument::synchronizationStates() const { return mSynchronizationStates; }

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
