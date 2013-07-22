/*
    This file is part of the Kasten Framework, made within the KDE community.

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
#include <KActionCollection>
#include <KToolBar>
#include <KLocalizedString>
#include <kdeversion.h>
// Qt
#include <QLayout>
#include <QUrl>
#include <QtCore/QDir>


namespace Kasten2
{

FileSystemBrowserView::FileSystemBrowserView( FileSystemBrowserTool* tool, QWidget* parent )
  : QWidget( parent ),
    mTool( tool )
{
    QMetaObject::invokeMethod( this, "init", Qt::QueuedConnection );
}

void FileSystemBrowserView::init()
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );

    // tool bar
    mToolbar = new KToolBar( this );
    mToolbar->setMovable( false );
    mToolbar->setToolButtonStyle( Qt::ToolButtonIconOnly );
    mToolbar->setIconDimensions( 16 );
    mToolbar->setContextMenuPolicy( Qt::NoContextMenu );
    layout->addWidget( mToolbar );

    // url bar
    KFilePlacesModel* filePlacesModel = new KFilePlacesModel( this );
    mUrlNavigator = new KUrlNavigator( filePlacesModel , QUrl::fromLocalFile( QDir::homePath() ), this );
    connect( mUrlNavigator, &KUrlNavigator::urlChanged, this, &FileSystemBrowserView::setDirOperatorUrl );
    layout->addWidget( mUrlNavigator );

    // view
    mDirOperator = new KDirOperator( QUrl::fromLocalFile( QDir::homePath() ), this );
    mDirOperator->setView( KFile::Detail );
    connect( mDirOperator, &KDirOperator::urlEntered, this, &FileSystemBrowserView::setNavigatorUrl );
    connect( mDirOperator, &KDirOperator::fileSelected, this, &FileSystemBrowserView::openFile );
    layout->addWidget( mDirOperator );

    // fill toolbar
    static const char* const ToolbarActionNames[] =
    { "back", "forward", "up", "home", "short view", "detailed view", "tree view"  };
    static const int ToolbarActionNamesCount = sizeof(ToolbarActionNames) / sizeof(ToolbarActionNames[0]);

    const KActionCollection* dirOperatorActionCollection = mDirOperator->actionCollection();
    for( int i = 0; i<ToolbarActionNamesCount; ++i )
    {
        QAction* action = dirOperatorActionCollection->action( QLatin1String(ToolbarActionNames[i]) );
        if( action )
            mToolbar->addAction( action );
    }

    mActionCollection = new KActionCollection( this );
    QAction* syncDirAction = mActionCollection->addAction( QLatin1String("sync_dir"),
                                                           this, SLOT(syncCurrentDocumentDirectory()) );
    syncDirAction->setIcon( QIcon::fromTheme( QStringLiteral("go-parent-folder") ) );
    syncDirAction->setText( i18nc("@action:intoolbar", "Folder of Current Document") );
    connect( mTool, &FileSystemBrowserTool::hasCurrentUrlChanged, syncDirAction, &QAction::setEnabled );
    syncDirAction->setEnabled( mTool->hasCurrentUrl() );
    mToolbar->addAction( syncDirAction );
}


void FileSystemBrowserView::setDirOperatorUrl( const QUrl& url )
{
    mDirOperator->setUrl( url, true );
}


void FileSystemBrowserView::setNavigatorUrl( const QUrl& url )
{
    mUrlNavigator->setLocationUrl( url );
}

void FileSystemBrowserView::syncCurrentDocumentDirectory()
{
    const QUrl url = mTool->currentUrl();

    if( !url.isEmpty() )
        setNavigatorUrl( url );
}

void FileSystemBrowserView::openFile( const KFileItem& fileItem )
{
    mTool->open( fileItem.url() );
}

FileSystemBrowserView::~FileSystemBrowserView() {}

}
