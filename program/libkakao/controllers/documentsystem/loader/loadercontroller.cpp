/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "loadercontroller.h"

// Kakao core
#include <kdocumentsyncmanager.h>
// KDE
#include <KUrl>
#include <KRecentFilesAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KXmlGuiWindow>
#include <KConfigGroup>
#include <KGlobal>

static const char AllFileNamesFilter[] = "*";
static const char CreatorConfigGroupId[] = "Recent Files";

LoaderController::LoaderController( KDocumentSyncManager *syncManager, KXmlGuiWindow *window )
: mSyncManager( syncManager ), mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    KStandardAction::open(       this, SLOT(load()),      actionCollection );
    mOpenRecentAction =
        KStandardAction::openRecent( this, SLOT(loadRecent( const KUrl& )), actionCollection );

    KConfigGroup configGroup( KGlobal::config(), CreatorConfigGroupId );
    mOpenRecentAction->loadEntries( configGroup );

    connect( mSyncManager, SIGNAL(urlUsed( const KUrl& )), SLOT(onUrlUsed( const KUrl& )) );
}


void LoaderController::setView( KAbstractView *view )
{
Q_UNUSED( view )
}

void LoaderController::load()
{
    mSyncManager->load();
}

void LoaderController::loadRecent( const KUrl &url )
{
    // TODO: 
    mSyncManager->load( url );
}

void LoaderController::onUrlUsed( const KUrl &url )
{
    mOpenRecentAction->addUrl( url );
}

LoaderController::~LoaderController()
{
    KConfigGroup configGroup( KGlobal::config(), CreatorConfigGroupId );
    mOpenRecentAction->saveEntries( configGroup );
}
