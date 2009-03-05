/*
    This file is part of the Kakao Framework, part of the KDE project.

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
#include "viewbox.h"
#include <kabstractdocument.h>
#include <kabstractview.h>
// KDE
#include <KTabWidget>


TabbedViews::TabbedViews()
{
    mTabWidget = new KTabWidget();

    mTabWidget->setCloseButtonEnabled( true );
    mTabWidget->setDocumentMode( true );

    connect( mTabWidget, SIGNAL(closeRequest( QWidget* )), SLOT(onCloseRequest( QWidget* )) );
    connect( mTabWidget, SIGNAL(currentChanged( int )), SLOT(onCurrentChanged( int )) );
    connect( mTabWidget, SIGNAL(testCanDecode( const QDragMoveEvent* , bool& )),
             SIGNAL(dragMove( const QDragMoveEvent* , bool& )) );
    connect( mTabWidget, SIGNAL(receivedDropEvent( QDropEvent* )),
             SIGNAL(drop( QDropEvent* )) );
}

QList<KAbstractView*> TabbedViews::viewList() const
{
    QList<KAbstractView*> result;

    const int count = mTabWidget->count();
    for( int i=0; i<count; ++i )
    {
        const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->widget(i) );
        KAbstractView* view = viewBox->view();
        result.append( view );
    }

    return result;
}

int TabbedViews::indexOf( KAbstractView* view ) const
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

void TabbedViews::addView( KAbstractView *view )
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

void TabbedViews::removeView( KAbstractView *view )
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

QWidget *TabbedViews::widget() const
{
    return mTabWidget;
}

void TabbedViews::setViewFocus( KAbstractView *view )
{
    const int index = indexOf( view );
    mTabWidget->setCurrentIndex( index );
}

KAbstractView *TabbedViews::viewFocus() const
{
    const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->currentWidget() );
    return viewBox ? viewBox->view() : 0;
}

void TabbedViews::onCurrentChanged( int index )
{
    const ViewBox* viewBox = static_cast<const ViewBox*>( mTabWidget->widget(index) );
    KAbstractView* view = viewBox ? viewBox->view() : 0;

    if( view )
        view->widget()->setFocus();

    emit viewFocusChanged( view );
}

void TabbedViews::onCloseRequest( QWidget* widget )
{
    const ViewBox* viewBox = static_cast<const ViewBox*>( widget );
    KAbstractView* view = viewBox->view();

    emit closeRequest( view );
}

void TabbedViews::onTitleChanged( const QString &newTitle )
{
    KAbstractView* view = qobject_cast<KAbstractView *>( sender() );
    if( view )
    {
        const int index = indexOf( view );
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
}
