/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "shellwindow_p.h"

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
#include <QtCore/QHash>


namespace Kasten
{

ShellWindowPrivate::ShellWindowPrivate( ShellWindow* parent,
                                        DocumentManager* documentManager,
                                        ViewManager* viewManager )
  : q_ptr( parent ),
    mGroupedViews( new MultiViewAreas() ),// TabbedViews() ),
    mCurrentView( 0 ),
    mDocumentManager( documentManager ),
    mViewManager( viewManager )
{
    parent->setCentralWidget( mGroupedViews->widget() );

    QObject::connect( mViewManager, SIGNAL(opened(QList<Kasten::AbstractView*>)),
                      mGroupedViews, SLOT(addViews(QList<Kasten::AbstractView*>)) );
    QObject::connect( mViewManager, SIGNAL(closing(QList<Kasten::AbstractView*>)),
                      mGroupedViews, SLOT(removeViews(QList<Kasten::AbstractView*>)) );

    QObject::connect( mGroupedViews, SIGNAL(viewFocusChanged(Kasten::AbstractView*)),
                      parent, SLOT(onViewFocusChanged(Kasten::AbstractView*)) );
    QObject::connect( mGroupedViews, SIGNAL(closeRequest(QList<Kasten::AbstractView*>)),
                      parent, SLOT(onCloseRequest(QList<Kasten::AbstractView*>)) );
    QObject::connect( mGroupedViews, SIGNAL(dataOffered(const QMimeData*,bool&)),
                      parent, SLOT(onDataOffered(const QMimeData*,bool&)) );
    QObject::connect( mGroupedViews, SIGNAL(dataDropped(const QMimeData*)),
                      parent, SLOT(onDataDropped(const QMimeData*)) );
}

void ShellWindowPrivate::addTool( AbstractToolView* toolView )
{
    Q_Q( ShellWindow );

    ToolViewDockWidget* dockWidget = new ToolViewDockWidget( toolView, q );
    // TODO: where to set the initial area?
    q->addDockWidget( Qt::RightDockWidgetArea, dockWidget );

    mTools.append( toolView->tool() );
    mDockWidgets.append( dockWidget );

    if( dockWidget->isVisible() && mCurrentView )
        toolView->tool()->setTargetModel( mCurrentView );

    QObject::connect( dockWidget, SIGNAL(visibilityChanged(bool)),
                      q, SLOT(onToolVisibilityChanged(bool)) );
}

void ShellWindowPrivate::showDocument( AbstractDocument* document )
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

void ShellWindowPrivate::updateControllers( AbstractView* view )
{
    foreach( AbstractXmlGuiController* controller, mControllers )
        controller->setTargetModel( view );

    foreach( ToolViewDockWidget* dockWidget, mDockWidgets )
    {
        if( dockWidget->isShown() )
            dockWidget->toolView()->tool()->setTargetModel( view );
    }
}

bool ShellWindowPrivate::queryClose()
{
    // TODO: query the document manager or query the view manager?
    return mDocumentManager->canCloseAll();
}

void ShellWindowPrivate::onTitleChanged( const QString& newTitle )
{
    Q_Q( ShellWindow );

    AbstractView* view = qobject_cast<AbstractView *>( q->sender() );
    if( view )
    {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        q->setCaption( newTitle, document->localSyncState() == LocalHasChanges );
    }
}

void ShellWindowPrivate::onLocalSyncStateChanged( LocalSyncState newState )
{
    Q_UNUSED( newState )
    Q_Q( ShellWindow );

    AbstractView* view = qobject_cast<AbstractView *>( q->sender() );
    if( view )
        q->setCaption( view->title(), newState == LocalHasChanges );
}

void ShellWindowPrivate::onViewFocusChanged( AbstractView *view )
{
    Q_Q( ShellWindow );

    if( mCurrentView ) mCurrentView->disconnect( q );

    mCurrentView = view;

    updateControllers( view );
    const QString title = view ? view->title() : QString();
    AbstractDocument* document = view ? view->findBaseModel<AbstractDocument*>() : 0;

    const bool changes = document ? document->localSyncState() == LocalHasChanges : false;
    q->setCaption( title, changes );

    if( view )
    {
        QObject::connect( view, SIGNAL(titleChanged(QString)),
                          q, SLOT(onTitleChanged(QString)) );
        QObject::connect( view, SIGNAL(localSyncStateChanged(Kasten::LocalSyncState)),
                          q, SLOT(onLocalSyncStateChanged(Kasten::LocalSyncState)) );
    }
}

void ShellWindowPrivate::onCloseRequest( const QList<Kasten::AbstractView*>& views )
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

void ShellWindowPrivate::onDataOffered( const QMimeData* mimeData, bool& accept )
{
    accept = KUrl::List::canDecode( mimeData )
             || mDocumentManager->createManager()->canCreateNewFromData( mimeData );
}

void ShellWindowPrivate::onDataDropped( const QMimeData* mimeData )
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

void ShellWindowPrivate::onToolVisibilityChanged( bool isVisible )
{
    Q_Q( ShellWindow );

    ToolViewDockWidget* dockWidget = qobject_cast<ToolViewDockWidget *>( q->sender() );
    if( dockWidget )
    {
        AbstractView* view = isVisible ? mCurrentView : 0;
        dockWidget->toolView()->tool()->setTargetModel( view );
    }
}

ShellWindowPrivate::~ShellWindowPrivate()
{
    // we have to explicitly reset any inline tool view before first deleting all tools
    // and then the grouped views, because on destruction of the inline tool view it
    // operates on the tool, which then has been no longer ->crash
    // The other option would be to first delete the view, but for reasons if do not
    // remember currently I prefer the destruction in this order
    // TODO: make this call unneeded
    mGroupedViews->setCurrentToolInlineView( 0 );

    qDeleteAll( mControllers );
    qDeleteAll( mDockWidgets );
    qDeleteAll( mTools );

    delete mGroupedViews;
}

}
