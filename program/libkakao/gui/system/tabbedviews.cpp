/***************************************************************************
                          tabbedviews.cpp  -  description
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


#include "tabbedviews.h"

// lib
#include <kabstractdocument.h>
#include <kabstractview.h>
#include <kviewmanager.h>
// Qt
#include <QtGui/QTabWidget>


TabbedViews::TabbedViews( KViewManager *viewManager )
 : mViewManager( viewManager )
{
    mTabWidget = new QTabWidget();

    connect( mTabWidget, SIGNAL(currentChanged( int )), SLOT(onCurrentChanged( int )) );
}


void TabbedViews::addView( KAbstractView *view )
{
    connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );

    const int index = mTabWidget->addTab( view->widget(), view->title() );
    mTabWidget->setCurrentIndex( index );
    view->widget()->setFocus();

    // fix for Qt bug:
    if( mTabWidget->count() == 1 )
        // simulate signal reaction
        onCurrentChanged( index );

    emit added( view );

}

void TabbedViews::removeView( KAbstractView *view )
{
    const int index = mTabWidget->indexOf( view->widget() );
    if( index != -1 )
        mTabWidget->removeTab( index );

    // fix for Qt bug: 
    const int currentIndex = mTabWidget->currentIndex();
    // last removed or no change in index?
    if( currentIndex == -1 || currentIndex == index )
        // simulate signal reaction
        onCurrentChanged( currentIndex );

    emit removing( view );
}

QWidget *TabbedViews::widget() const
{
    return mTabWidget;
}

void TabbedViews::setViewFocus( KAbstractView *view )
{
    const int index = mTabWidget->indexOf( view->widget() );
    mTabWidget->setCurrentIndex( index );
}

KAbstractView *TabbedViews::viewFocus() const
{
    return mViewManager->viewByWidget( mTabWidget->currentWidget() );
}

void TabbedViews::onCurrentChanged( int index )
{
    QWidget *Widget = mTabWidget->widget( index );
    KAbstractView *view = mViewManager->viewByWidget( Widget );

    emit viewFocusChanged( view );
}

void TabbedViews::onTitleChanged( const QString &newTitle )
{
    KAbstractView* view = qobject_cast<KAbstractView *>( sender() );
    if( view )
    {
        const int index = mTabWidget->indexOf( view->widget() );
        if( index != -1 )
            mTabWidget->setTabText( index, newTitle );
    }

}

#if 0
void TabbedViews::onModifiedChanged( KAbstractDocument::SynchronizationStates newStates )
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
#endif

TabbedViews::~TabbedViews()
{
}
