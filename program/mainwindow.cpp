/***************************************************************************
                          mainwindow.cpp  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "mainwindow.h"

// program
#include "program.h"
// controllers
#include <poddecoder/poddecodercontroller.h>
// #include <chartable/chartablecontroller.h>
// #include <statistic/statisticcontroller.h>
#include <insertpattern/insertpatterncontroller.h>
#include <gotooffset/gotooffsetcontroller.h>
#include <search/searchcontroller.h>
#include <replace/replacecontroller.h>
#include <viewconfig/viewconfigcontroller.h>
// kakao controllers
#include <documentsystem/creator/creatorcontroller.h>
#include <document/synchronize/synchronizecontroller.h>
#include <view/zoom/zoomcontroller.h>
#include <view/select/selectcontroller.h>
#include <view/clipboard/clipboardcontroller.h>
#include <shellwindow/viewlistmenu/viewlistmenucontroller.h>
#include <shellwindow/fullscreen/fullscreencontroller.h>
#include <viewsystem/close/closecontroller.h>
#include <program/quit/quitcontroller.h>
// kakao
#include <kabstractdocument.h>
#include <kdocumentmanager.h>
#include <kdocumentcreator.h>
#include <kabstractview.h>
#include <kviewmanager.h>
// Qt
#include <QtGui/QTabWidget>


OktetaMainWindow::OktetaMainWindow( OktetaProgram *program )
 : mProgram( program )
{
  mViewsTab = new QTabWidget( this );
  setCentralWidget( mViewsTab );

  setupControllers();
  setupGUI();

  connect( mViewsTab, SIGNAL(currentChanged( int )), SLOT(onCurrentChanged( int )) );
}

void OktetaMainWindow::setupControllers()
{
  mControllers.append( new CreatorController(mProgram->documentManager()->creator(),this) );
  mControllers.append( new SynchronizeController(mProgram->documentManager()->synchronizer(),this) );
  mControllers.append( new CloseController(mProgram->viewManager(),this) );
  mControllers.append( new ViewListMenuController(mProgram->viewManager(),this,this) );
  mControllers.append( new FullScreenController(this) );
  mControllers.append( new QuitController(0,this) );

  mControllers.append( new ZoomController(this) );
  mControllers.append( new SelectController(this) );
  mControllers.append( new ClipboardController(this) );

  mControllers.append( new PODDecoderController(mProgram->viewManager(),this) );
//   mControllers.append( new CharTableController(this) );
//   mControllers.append( new StatisticController(this) );
  mControllers.append( new SearchController(this) );
  mControllers.append( new ReplaceController(this) );
  mControllers.append( new GotoOffsetController(this) );
  mControllers.append( new InsertPatternController(this) );
  mControllers.append( new ViewConfigController(this) );
}

void OktetaMainWindow::setViewFocus( KAbstractView *view )
{
    const int index = mViewsTab->indexOf( view->widget() );
    mViewsTab->setCurrentIndex( index );
}

KAbstractView *OktetaMainWindow::viewFocus() const
{
    return mProgram->viewManager()->viewByWidget( mViewsTab->currentWidget() );
}


void OktetaMainWindow::addView( KAbstractView *view )
{
    const int index = mViewsTab->addTab( view->widget(), view->title() );
    mViewsTab->setCurrentIndex( index );
    view->widget()->setFocus();

    connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );
    connect( view, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                   SLOT(onModifiedChanged( KAbstractDocument::SynchronizationStates )) );

    // fix for Qt bug:
    if( mViewsTab->count() == 1 )
        // simulate signal reaction
        onCurrentChanged( index );
}

void OktetaMainWindow::removeView( KAbstractView *view )
{
    const int index = mViewsTab->indexOf( view->widget() );
    if( index != -1 )
        mViewsTab->removeTab( index );

    // fix for Qt bug: 
    const int currentIndex = mViewsTab->currentIndex();
    // last removed or no change in index?
    if( currentIndex == -1 || currentIndex == index )
        // simulate signal reaction
        onCurrentChanged( currentIndex );
}

void OktetaMainWindow::onTitleChanged( const QString &newTitle )
{
    KAbstractView* view = qobject_cast<KAbstractView *>( sender() );
    if( view )
    {
        const int index = mViewsTab->indexOf( view->widget() );
        if( index != -1 )
        {
            mViewsTab->setTabText( index, newTitle );
            if( index == mViewsTab->currentIndex() )
                setCaption( view->title(), view->document()->hasLocalChanges() );
        }
    }

}

void OktetaMainWindow::onModifiedChanged( KAbstractDocument::SynchronizationStates newStates )
{
Q_UNUSED( newStates )
    KAbstractView* view = qobject_cast<KAbstractView *>( sender() );
    if( view )
    {
        const int index = mViewsTab->indexOf( view->widget() );
        if( index != -1 )
        {
//             mViewsTab->setIcon( index, newTitle ); //modificationSymbol
            if( index == mViewsTab->currentIndex() )
                setCaption( view->title(), view->document()->hasLocalChanges() );
        }
    }

}

void OktetaMainWindow::onCurrentChanged( int index )
{
    QWidget *Widget = mViewsTab->widget( index );
    KAbstractView *view = mProgram->viewManager()->viewByWidget( Widget );

    updateControllers( view );
    const QString title = view ? view->title() : QString();
    const bool changes = view ? view->document()->hasLocalChanges() : false;
    setCaption( title, changes );

    emit viewFocusChanged( view );
}

void OktetaMainWindow::updateControllers( KAbstractView* view )
{
    QList<KViewController*>::Iterator it;
    QList<KViewController*>::Iterator end = mControllers.end();
    for( it=mControllers.begin(); it != end; ++it )
        (*it)->setView( view );
}

bool OktetaMainWindow::queryClose()
{
    // TODO: query the document manager or query the view manager?
    return mProgram->documentManager()->canCloseAll();
}

OktetaMainWindow::~OktetaMainWindow()
{
    QList<KViewController*>::Iterator it;
    QList<KViewController*>::Iterator end = mControllers.end();
    for( it=mControllers.begin(); it != end; ++it )
        delete *it;
}

#include "mainwindow.moc"
