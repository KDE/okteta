/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <multiviewareas.h>
#include <tabbedviews.h>
#include <viewmanager.h>
#include <abstractview.h>
#include <abstracttoolview.h>
#include <abstracttool.h>
#include <abstractxmlguicontroller.h>
#include <toolviewdockwidget.h>
// Kasten core
#include <documentmanager.h>
#include <documentcreatemanager.h>
#include <documentsyncmanager.h>
#include <abstractdocument.h>
// KDE
#include <KUrl>
// Qt
#include <QtGui/QDockWidget>
#include <QtCore/QHash>


namespace Kasten
{

ShellWindow::ShellWindow( DocumentManager* documentManager, ViewManager* viewManager )
  : mGroupedViews( new MultiViewAreas() ),// TabbedViews() ),
    mCurrentView( 0 ),
    mDocumentManager( documentManager ),
    mViewManager( viewManager )
{
    setCentralWidget( mGroupedViews->widget() );

    connect( mDocumentManager, SIGNAL(added( const QList<Kasten::AbstractDocument*>& )),
             mViewManager, SLOT(createViewsFor( const QList<Kasten::AbstractDocument*>& )) );
    connect( mDocumentManager, SIGNAL(closing( const QList<Kasten::AbstractDocument*>& )),
             mViewManager, SLOT(removeViewsFor( const QList<Kasten::AbstractDocument*>& )) );

    connect( mViewManager, SIGNAL(opened( const QList<Kasten::AbstractView*>& )),
             mGroupedViews, SLOT(addViews( const QList<Kasten::AbstractView*>& )) );
    connect( mViewManager, SIGNAL(closing( const QList<Kasten::AbstractView*>& )),
             mGroupedViews, SLOT(removeViews( const QList<Kasten::AbstractView*>& )) );

    connect( mDocumentManager, SIGNAL(focusRequested( Kasten::AbstractDocument* )),
             SLOT(onFocusRequested( Kasten::AbstractDocument* )) );

    connect( mGroupedViews, SIGNAL(viewFocusChanged( Kasten::AbstractView* )),
             SLOT(onViewFocusChanged( Kasten::AbstractView* )) );
    connect( mGroupedViews, SIGNAL(closeRequest( const QList<Kasten::AbstractView*>& )),
             SLOT(onCloseRequest( const QList<Kasten::AbstractView*>& )) );
    connect( mGroupedViews, SIGNAL(dataOffered( const QMimeData*, bool& )),
             SLOT(onDataOffered( const QMimeData*, bool& )) );
    connect( mGroupedViews, SIGNAL(dataDropped( const QMimeData* )),
             SLOT(onDataDropped( const QMimeData* )) );
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

void ShellWindow::updateControllers( AbstractView* view )
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
    AbstractView* view = qobject_cast<AbstractView *>( sender() );
    if( view )
    {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        setCaption( newTitle, document->hasLocalChanges() );
    }
}

void ShellWindow::onSyncStatesChanged( AbstractDocument::SyncStates newStates )
{
Q_UNUSED( newStates )
    AbstractView* view = qobject_cast<AbstractView *>( sender() );
    if( view )
    {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        setCaption( view->title(), document->hasLocalChanges() );
    }
}

void ShellWindow::onViewFocusChanged( AbstractView *view )
{
    if( mCurrentView ) mCurrentView->disconnect( this );

    mCurrentView = view;

    updateControllers( view );
    const QString title = view ? view->title() : QString();
    AbstractDocument* document = view ? view->findBaseModel<AbstractDocument*>() : 0;

    const bool changes = document ? document->hasLocalChanges() : false;
    setCaption( title, changes );

    if( view )
    {
        connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );
        connect( view, SIGNAL(syncStatesChanged( Kasten::AbstractDocument::SyncStates )),
                       SLOT(onSyncStatesChanged( Kasten::AbstractDocument::SyncStates )) );
    }
}

void ShellWindow::onFocusRequested( AbstractDocument* document )
{
    AbstractGroupedViews* currentGroupedViews = static_cast<AbstractGroupedViews*>( mGroupedViews->viewAreaFocus() );
    const QList<AbstractView*> viewList = currentGroupedViews->viewList();

    AbstractView* viewOfDocument = 0;
    foreach( AbstractView* view, viewList )
    {
        if( view->findBaseModel<AbstractDocument*>() == document )
        {
            viewOfDocument = view;
            break;
        }
    }

    if( viewOfDocument )
        mGroupedViews->setViewFocus( viewOfDocument );
    else
    {
        QList<Kasten::AbstractDocument*> documents;
        documents.append( document );
        mViewManager->createViewsFor( documents );
    }
}

void ShellWindow::onCloseRequest( const QList<Kasten::AbstractView*>& views )
{
    // group views per document
    QHash<AbstractDocument*,QList<AbstractView*> > viewsToClosePerDocument;
    foreach( AbstractView* view, views )
    {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        viewsToClosePerDocument[document].append( view );
    }

    // find documents which lose all views
    const QList<AbstractView*> allViews = mViewManager->views();
    foreach( AbstractView* view, allViews )
    {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        QHash<AbstractDocument*,QList<AbstractView*> >::Iterator it =
            viewsToClosePerDocument.find( document );

        if( it != viewsToClosePerDocument.end() )
        {
            const QList<AbstractView*>& viewsOfDocument = it.value();
            const bool isAnotherView = ! viewsOfDocument.contains( view );
            if( isAnotherView )
                viewsToClosePerDocument.erase( it );
        }
    }

    const QList<AbstractDocument*> documentsWithoutViews = viewsToClosePerDocument.keys();

    if( mDocumentManager->canClose(documentsWithoutViews) )
    {
        mViewManager->removeViews( views );
        mDocumentManager->closeDocuments( documentsWithoutViews );
    }
}

void ShellWindow::onDataOffered( const QMimeData* mimeData, bool& accept )
{
    accept = KUrl::List::canDecode( mimeData )
             || mDocumentManager->createManager()->canCreateNewFromData( mimeData );
}

void ShellWindow::onDataDropped( const QMimeData* mimeData )
{
    const KUrl::List urls = KUrl::List::fromMimeData( mimeData );

    if( ! urls.isEmpty() )
    {
        DocumentSyncManager* syncManager = mDocumentManager->syncManager();

        foreach( const KUrl& url, urls )
            syncManager->load( url );
    }
    else
        mDocumentManager->createManager()->createNewFromData( mimeData, true );
}

void ShellWindow::onToolVisibilityChanged( bool isVisible )
{
    ToolViewDockWidget* dockWidget = qobject_cast<ToolViewDockWidget *>( sender() );
    if( dockWidget )
    {
        AbstractView* view = isVisible ? mCurrentView : 0;
        dockWidget->toolView()->tool()->setTargetModel( view );
    }
}

ShellWindow::~ShellWindow()
{
    // we have to explicitly reset any inline tool view before first deleting all tools
    // and then the grouped views, because on destruction of the inline tool view it
    // operates on the tool, which then has been no longer ->crash
    // The other option would be to first delete the view, but for reasons if do not
    // remember currently I prefer the destruction in this order
    // TODO: make this call unneeded
    mGroupedViews->setCurrentToolInlineView( 0 );

    qDeleteAll( mControllers );
    qDeleteAll( mTools );

    delete mGroupedViews;
}

}
