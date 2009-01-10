/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "shellwindow.h"

// lib
#include <tabbedviews.h>
#include <kviewmanager.h>
#include <kabstractview.h>
#include <abstracttoolview.h>
#include <abstracttool.h>
#include <abstractxmlguicontroller.h>
#include <toolviewdockwidget.h>
// Kakao core
#include <kdocumentmanager.h>
#include <kdocumentsyncmanager.h>
#include <kabstractdocument.h>
// KDE
#include <KUrl>
// Qt
#include <QtGui/QDockWidget>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>


ShellWindow::ShellWindow( KDocumentManager *documentManager, KViewManager *viewManager )
 : mGroupedViews( new TabbedViews(viewManager) ), mCurrentView( 0 ),
   mDocumentManager( documentManager ), mViewManager( viewManager )
{
    setCentralWidget( mGroupedViews->widget() );

    mViewManager->setWindow( this );
    connect( mViewManager, SIGNAL(opened( KAbstractView* )),
             mGroupedViews, SLOT(addView( KAbstractView* )) );
    connect( mViewManager, SIGNAL(closing( KAbstractView* )),
             mGroupedViews, SLOT(removeView( KAbstractView* )) );

    connect( mDocumentManager, SIGNAL(focusRequested( KAbstractDocument* )),
             SLOT(onFocusRequested( KAbstractDocument* )) );

    connect( mGroupedViews, SIGNAL(viewFocusChanged( KAbstractView* )),
             SLOT(onViewFocusChanged( KAbstractView* )) );
    connect( mGroupedViews, SIGNAL(closeRequest( KAbstractView* )),
             SLOT(onCloseRequest( KAbstractView* )) );
    connect( mGroupedViews, SIGNAL(dragMove( const QDragMoveEvent*, bool& )),
             SLOT(onDragMoveEvent( const QDragMoveEvent*, bool& )) );
    connect( mGroupedViews, SIGNAL(drop( QDropEvent* )),
             SLOT(onDropEvent( QDropEvent* )) );
}

QList<ToolViewDockWidget*> ShellWindow::dockWidgets() const { return mDockWidgets; }

void ShellWindow::addXmlGuiController( AbstractXmlGuiController* controller )
{
    mControllers.append( controller );
}

void ShellWindow::addTool( AbstractToolView* toolView )
{
    ToolViewDockWidget* dockWidget = new ToolViewDockWidget( toolView, this );
    // TODO: where to set the initial area?
    addDockWidget( Qt::RightDockWidgetArea, dockWidget );

    mTools.append( toolView->tool() );
    mDockWidgets.append( dockWidget );

    if( dockWidget->isVisible() && mCurrentView )
        toolView->tool()->setTargetModel( mCurrentView );

    connect( dockWidget, SIGNAL(visibilityChanged( bool )), SLOT(onToolVisibilityChanged( bool )) );
}

void ShellWindow::updateControllers( KAbstractView* view )
{
    foreach( AbstractXmlGuiController* controller, mControllers )
        controller->setTargetModel( view );

    foreach( ToolViewDockWidget* dockWidget, mDockWidgets )
    {
        if( dockWidget->isShown() )
            dockWidget->toolView()->tool()->setTargetModel( view );
    }
}

bool ShellWindow::queryClose()
{
    // TODO: query the document manager or query the view manager?
    return mDocumentManager->canCloseAll();
}

void ShellWindow::onTitleChanged( const QString &newTitle )
{
    KAbstractView* view = qobject_cast<KAbstractView *>( sender() );
    if( view )
        setCaption( newTitle, view->document()->hasLocalChanges() );
}

void ShellWindow::onModifiedChanged( KAbstractDocument::SynchronizationStates newStates )
{
Q_UNUSED( newStates )
    KAbstractView* view = qobject_cast<KAbstractView *>( sender() );
    if( view )
        setCaption( view->title(), view->document()->hasLocalChanges() );
}

void ShellWindow::onViewFocusChanged( KAbstractView *view )
{
    if( mCurrentView ) mCurrentView->disconnect( this );

    mCurrentView = view;

    updateControllers( view );
    const QString title = view ? view->title() : QString();
    const bool changes = view ? view->document()->hasLocalChanges() : false;
    setCaption( title, changes );

    if( view )
    {
        connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );
        connect( view, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                       SLOT(onModifiedChanged( KAbstractDocument::SynchronizationStates )) );
    }
}

void ShellWindow::onFocusRequested( KAbstractDocument* document )
{
    KAbstractView* view = mViewManager->viewOfDocument( document );
    if( view )
        mGroupedViews->setViewFocus( view );
}

void ShellWindow::onCloseRequest( KAbstractView* view )
{
    KAbstractDocument* document = view->document();

    if( mDocumentManager->canClose(document) )
        mDocumentManager->closeDocument( document );
}

void ShellWindow::onDragMoveEvent( const QDragMoveEvent* event, bool& accept )
{
    accept = KUrl::List::canDecode( event->mimeData() );
}

void ShellWindow::onDropEvent( QDropEvent* event )
{
    const KUrl::List urls = KUrl::List::fromMimeData( event->mimeData() );
    if( !urls.isEmpty() )
    {
        KDocumentSyncManager* syncManager = mDocumentManager->syncManager();

        foreach( const KUrl& url, urls )
            syncManager->load( url );
    }
}

void ShellWindow::onToolVisibilityChanged( bool isVisible )
{
    ToolViewDockWidget* dockWidget = qobject_cast<ToolViewDockWidget *>( sender() );
    if( dockWidget )
    {
        KAbstractView* view = isVisible ? mCurrentView : 0;
        dockWidget->toolView()->tool()->setTargetModel( view );
    }
}

ShellWindow::~ShellWindow()
{
    qDeleteAll( mControllers );
    qDeleteAll( mTools );

    delete mGroupedViews;
}
