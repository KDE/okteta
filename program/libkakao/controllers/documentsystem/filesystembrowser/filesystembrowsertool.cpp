/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "filesystembrowsertool.h"

// Kakao core
#include <kdocumentmanager.h>
#include <kdocumentsyncmanager.h>
#include <kiversionable.h>
#include <kabstractdocument.h>
#include <abstractmodelsynchronizer.h>
// KDE
#include <KLocale>
#include <KUrl>


FileSystemBrowserTool::FileSystemBrowserTool( KDocumentManager* documentManager )
 : mDocument( 0 ), mDocumentManager( documentManager )
{
    setObjectName( "FileSystemBrowser" );
}


QString FileSystemBrowserTool::title() const { return i18nc("@title:window", "Filesystem"); }


KUrl FileSystemBrowserTool::currentUrl() const
{
    KUrl result;

    if( mDocument )
        result = mDocumentManager->syncManager()->urlOf( mDocument ).upUrl();

    return result;
}

bool FileSystemBrowserTool::hasCurrentUrl() const
{
    return ( mDocument && mDocument->synchronizer() != 0 );
}

void FileSystemBrowserTool::setTargetModel( AbstractModel* model )
{
    const bool oldHasCurrentUrl = hasCurrentUrl();

    mDocument = model ? model->findBaseModel<KAbstractDocument*>() : 0;

    const bool newHasCurrentUrl = hasCurrentUrl();

    if( oldHasCurrentUrl != newHasCurrentUrl )
        emit hasCurrentUrlChanged( newHasCurrentUrl );
}

void FileSystemBrowserTool::open( const KUrl& url )
{
    mDocumentManager->syncManager()->load( url );
}

FileSystemBrowserTool::~FileSystemBrowserTool() {}
