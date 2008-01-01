/***************************************************************************
                          shellwindow.cpp  -  description
                             -------------------
    begin                : Wed Oct 24 2007
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


#include "shellwindow.h"

// kakao
#include <kabstractdocument.h>
#include <kdocumentmanager.h>
#include <kabstractview.h>
#include <kviewmanager.h>
#include <tabbedviews.h>


ShellWindow::ShellWindow( KDocumentManager *documentManager, KViewManager *viewManager )
 : mTabbedViews( new TabbedViews(viewManager) ), mCurrentView( 0 ),
   mDocumentManager( documentManager ), mViewManager( viewManager )
{
    setCentralWidget( mTabbedViews->widget() );

    mViewManager->setWindow( this );
    connect( mViewManager, SIGNAL(opened( KAbstractView* )),
             mTabbedViews, SLOT(addView( KAbstractView* )) );
    connect( mViewManager, SIGNAL(closing( KAbstractView* )),
             mTabbedViews, SLOT(removeView( KAbstractView* )) );

    connect( mTabbedViews, SIGNAL(viewFocusChanged( KAbstractView* )), SLOT(onViewFocusChanged( KAbstractView* )) );
}

void ShellWindow::updateControllers( KAbstractView* view )
{
    QList<KViewController*>::Iterator it;
    QList<KViewController*>::Iterator end = mControllers.end();
    for( it=mControllers.begin(); it != end; ++it )
        (*it)->setView( view );
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
    mCurrentView->disconnect( this );

    mCurrentView = view;

    updateControllers( view );
    const QString title = view ? view->title() : QString();
    const bool changes = view ? view->document()->hasLocalChanges() : false;
    setCaption( title, changes );

    connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );
    connect( view, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                   SLOT(onModifiedChanged( KAbstractDocument::SynchronizationStates )) );
}

ShellWindow::~ShellWindow()
{
    QList<KViewController*>::Iterator it;
    QList<KViewController*>::Iterator end = mControllers.end();
    for( it=mControllers.begin(); it != end; ++it )
        delete *it;

    delete mTabbedViews;
}
