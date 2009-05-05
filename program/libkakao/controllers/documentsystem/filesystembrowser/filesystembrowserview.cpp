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

#include "filesystembrowserview.h"

// lib
#include "filesystembrowsertool.h"
// KDE
#include <KUrlNavigator>
#include <KFilePlacesModel>
#include <KDirOperator>
// Qt
#include <QtGui/QLayout>
#include <QtCore/QDir>


FileSystemBrowserView::FileSystemBrowserView( FileSystemBrowserTool* tool, QWidget* parent )
  : QWidget( parent ),
    mTool( tool )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    KFilePlacesModel* filePlacesModel = new KFilePlacesModel( this );
    mUrlNavigator = new KUrlNavigator( filePlacesModel , KUrl( QDir::homePath() ), this );
    connect( mUrlNavigator, SIGNAL(urlChanged( const KUrl& )), SLOT(setDirOperatorUrl( const KUrl& )) );
    layout->addWidget( mUrlNavigator );

    mDirOperator = new KDirOperator( QDir::homePath(), this );
    mDirOperator->setView( KFile::Tree );
    connect( mDirOperator, SIGNAL(urlEntered( const KUrl& )), SLOT(setNavigatorUrl( const KUrl& )));
    connect( mDirOperator, SIGNAL(fileSelected( const KFileItem& )), SLOT(openFile( const KFileItem& )) );
    layout->addWidget( mDirOperator );
}


void FileSystemBrowserView::setDirOperatorUrl( const KUrl& url )
{
    mDirOperator->setUrl( url, true );
}


void FileSystemBrowserView::setNavigatorUrl( const KUrl& url )
{
    mUrlNavigator->setUrl( url );
}

void FileSystemBrowserView::syncCurrentDocumentDirectory()
{
    const KUrl url = mTool->urlOfDirOfCurrentDocument();

    if( !url.isEmpty() )
        setNavigatorUrl( url );
}

void FileSystemBrowserView::openFile( const KFileItem& fileItem )
{
    mTool->open( fileItem.url() );
}

FileSystemBrowserView::~FileSystemBrowserView() {}
