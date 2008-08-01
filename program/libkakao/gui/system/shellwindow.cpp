/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// Kakao core
#include <kdocumentmanager.h>
#include <kabstractdocument.h>


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
    foreach( KViewController* controller, mControllers )
        controller->setView( view );
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

ShellWindow::~ShellWindow()
{
    foreach( KViewController* controller, mControllers )
        delete controller;

    delete mTabbedViews;
}
