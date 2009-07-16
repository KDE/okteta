/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "tabbedviews.h"

// lib
#include "tabbedviewsbox.h"
#include "viewbox.h"
#include "toolinlineviewwidget.h"
#include <abstracttoolinlineview.h>
#include <abstractdocument.h>
#include <abstractview.h>
// KDE
#include <KTabWidget>


namespace Kasten
{

TabbedViews::TabbedViews()
{
    mTabbedViewsBox = new TabbedViewsBox();

    mTabWidget = mTabbedViewsBox->tabWidget();
    mTabWidget->setCloseButtonEnabled( true );
    mTabWidget->setDocumentMode( true );

    connect( mTabWidget, SIGNAL(closeRequest( QWidget* )), SLOT(onCloseRequest( QWidget* )) );
    connect( mTabWidget, SIGNAL(currentChanged( int )), SLOT(onCurrentChanged( int )) );
    connect( mTabWidget, SIGNAL(testCanDecode( const QDragMoveEvent* , bool& )),
             SIGNAL(dragMove( const QDragMoveEvent* , bool& )) );
    connect( mTabWidget, SIGNAL(receivedDropEvent( QDropEvent* )),
             SIGNAL(drop( QDropEvent* )) );
}

QList<AbstractView*> TabbedViews::viewList() const
{
    QList<AbstractView*> result;

    const int count = mTabWidget->count();
    for( int i=0; i<count; ++i )
    {
        const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->widget(i) );
        AbstractView* view = viewBox->view();
        result.append( view );
    }

    return result;
}

int TabbedViews::indexOf( AbstractView* view ) const
{
    int result = -1;

    const int tabCount = mTabWidget->count();
    for( int i=0; i<tabCount; ++i )
    {
        const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->widget(i) );
        if( view == viewBox->view() )
        {
            result = i;
            break;
        }
    }

    return result;
}

void TabbedViews::addView( AbstractView *view )
{
    connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );

    ViewBox* viewBox = new ViewBox( view, mTabWidget );
    const int index = mTabWidget->addTab( viewBox, view->title() );
    mTabWidget->setCurrentIndex( index );
    view->widget()->setFocus();

    // fix for Qt bug:
    if( mTabWidget->count() == 1 )
        // simulate signal reaction
        onCurrentChanged( index );

    emit added( view );

}

void TabbedViews::removeView( AbstractView *view )
{
    view->disconnect( this );

    const int index = indexOf( view );
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

void TabbedViews::addToolInlineView( AbstractToolInlineView* view )
{
    mToolInlineViewList.append( view );
}

void TabbedViews::setCurrentToolInlineView( AbstractToolInlineView* view )
{
    ToolInlineViewWidget* currentViewWidget =
        qobject_cast<ToolInlineViewWidget*>( mTabbedViewsBox->bottomWidget() );

    if( ! currentViewWidget || (currentViewWidget->view() != view) )
    {
        ToolInlineViewWidget* toolInlineViewWidget = new ToolInlineViewWidget( view/*->widget()*/ );
        mTabbedViewsBox->setBottomWidget( toolInlineViewWidget );
    }

    view->widget()->setFocus();
}

QWidget* TabbedViews::widget() const
{
    return mTabbedViewsBox;
}

void TabbedViews::setViewFocus( AbstractView *view )
{
    const int index = indexOf( view );
    mTabWidget->setCurrentIndex( index );
}

AbstractView *TabbedViews::viewFocus() const
{
    const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->currentWidget() );
    return viewBox ? viewBox->view() : 0;
}

void TabbedViews::onCurrentChanged( int index )
{
    mTabbedViewsBox->setBottomWidget( 0 );

    const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->widget(index) );
    AbstractView* view = viewBox ? viewBox->view() : 0;

    if( view )
        view->widget()->setFocus();

    emit viewFocusChanged( view );
}

void TabbedViews::onCloseRequest( QWidget* widget )
{
    const ViewBox* viewBox = static_cast<const ViewBox*>( widget );
    AbstractView* view = viewBox->view();

    emit closeRequest( view );
}

void TabbedViews::onTitleChanged( const QString &newTitle )
{
    AbstractView* view = qobject_cast<AbstractView *>( sender() );
    if( view )
    {
        const int index = indexOf( view );
        if( index != -1 )
            mTabWidget->setTabText( index, newTitle );
    }
}


#if 0
void TabbedViews::onModifiedChanged( AbstractDocument::SynchronizationStates newStates )
{
Q_UNUSED( newStates )
    AbstractView* view = qobject_cast<AbstractView *>( sender() );
    if( view )
    {
        const int index = indexOf( view );
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
    delete mTabbedViewsBox;
}

}
