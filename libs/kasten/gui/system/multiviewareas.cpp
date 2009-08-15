/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#include "multiviewareas.h"

// lib
#include "tabbedviews.h"
#include "viewbox.h"
#include "toolinlineviewwidget.h"
#include <abstracttoolinlineview.h>
#include <abstractdocument.h>
#include <abstractview.h>
// KDE
#include <KGlobalSettings>
// Qt
#include <QtGui/QSplitter>
#include <KDebug>

namespace Kasten
{
// TODO: catch area focues change!
MultiViewAreas::MultiViewAreas()
{
    mMainSplitter = new QSplitter();

    TabbedViews* viewArea = new TabbedViews();
    connect( viewArea, SIGNAL(focusChanged( bool )),
             SLOT(onViewAreaFocusChanged( bool )) );
    connect( viewArea, SIGNAL(viewFocusChanged( Kasten::AbstractView* )),
             SIGNAL(viewFocusChanged( Kasten::AbstractView* )) );
    connect( viewArea, SIGNAL(closeRequest( const QList<Kasten::AbstractView*>& )),
             SIGNAL(closeRequest( const QList<Kasten::AbstractView*>& )) );
    connect( viewArea, SIGNAL(removing( const QList<Kasten::AbstractView*>& )),
             SLOT(onViewsRemoved()) );
    mViewAreaList.append( viewArea );
    mCurrentViewArea = viewArea;

    mMainSplitter->setOpaqueResize( KGlobalSettings::opaqueResize() );
    mMainSplitter->addWidget( viewArea->widget() );
}

QList<AbstractView*> MultiViewAreas::viewList() const
{
    QList<AbstractView*> result;

    foreach( TabbedViews* viewArea, mViewAreaList )
        result.append( viewArea->viewList() );

    return result;
}
int MultiViewAreas::viewCount() const
{
    int result = 0;

    foreach( TabbedViews* viewArea, mViewAreaList )
        result += viewArea->viewCount();

    return result;
}

int MultiViewAreas::indexOf( AbstractView* view ) const
{
    int result = -1;

    int globalBaseIndex = 0;
    foreach( TabbedViews* viewArea, mViewAreaList )
    {
        const int localIndexOf = viewArea->indexOf( view );
        if( localIndexOf != -1 )
        {
            result = globalBaseIndex + localIndexOf;
            break;
        }
        globalBaseIndex += viewArea->viewCount();
    }

    return result;
}

QWidget* MultiViewAreas::widget() const { return mMainSplitter; }
bool MultiViewAreas::hasFocus() const { return mCurrentViewArea->hasFocus(); }

AbstractView* MultiViewAreas::viewFocus() const
{
    return mCurrentViewArea->viewFocus();
}

AbstractViewArea* MultiViewAreas::viewAreaFocus() const
{
    return mCurrentViewArea;
}

int MultiViewAreas::viewAreasCount() const { return mViewAreaList.count(); }


void MultiViewAreas::setFocus()
{
    mCurrentViewArea->setFocus();
}

void MultiViewAreas::addViews( const QList<AbstractView*>& views )
{
    mCurrentViewArea->addViews( views );

    emit added( views );
}

void MultiViewAreas::removeViews( const QList<AbstractView*>& views )
{
    // TODO: possible to just send the views of the given area?
    foreach( TabbedViews* viewArea, mViewAreaList )
        viewArea->removeViews( views );

    // TODO: above might trigger removal of areas before, is this a problem?
    emit removing( views );
}
#if 0
void MultiViewAreas::addToolInlineView( AbstractToolInlineView* view )
{
    mToolInlineViewList.append( view );
}

void MultiViewAreas::setCurrentToolInlineView( AbstractToolInlineView* view )
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
#endif
void MultiViewAreas::setViewFocus( AbstractView *view )
{
    // TODO: makes this more efficient!
    foreach( TabbedViews* viewArea, mViewAreaList )
    {
        const int localIndex = viewArea->indexOf( view );
        if( localIndex != -1 )
        {
            viewArea->setViewFocus( view );
            break;
        }
    }
}

void MultiViewAreas::setViewAreaFocus( AbstractViewArea* _viewArea )
{
    TabbedViews* viewArea = static_cast<TabbedViews*>( _viewArea );
    if( viewArea == mCurrentViewArea )
        return;

    if( mViewAreaList.contains(viewArea) )
        viewArea->setViewFocus( viewArea->viewFocus() );
}

void MultiViewAreas::splitViewArea( AbstractViewArea* _viewArea, Qt::Orientation orientation )
{
    TabbedViews* firstViewArea = static_cast<TabbedViews*>( _viewArea );
    QWidget* firstViewAreaWidget = firstViewArea->widget();
    QSplitter* baseSplitter = static_cast<QSplitter*>( firstViewAreaWidget->parentWidget() );

    QSplitter* splitter;
    if( baseSplitter->count() == 1 ) // only valid with mMainSplitter
        splitter = baseSplitter;
    else
    {
        const QList<int> baseSplitterSizes = baseSplitter->sizes();
        const int index = baseSplitter->indexOf( firstViewAreaWidget );
        splitter = new QSplitter( baseSplitter );
        // TODO: react on opaqueResize change
        splitter->setOpaqueResize( KGlobalSettings::opaqueResize() );
        baseSplitter->insertWidget( index, splitter );
        splitter->addWidget( firstViewAreaWidget );
        baseSplitter->setSizes( baseSplitterSizes );
    }

    TabbedViews* secondViewArea = new TabbedViews();
    connect( secondViewArea, SIGNAL(focusChanged( bool )),
             SLOT(onViewAreaFocusChanged( bool )) );
    connect( secondViewArea, SIGNAL(viewFocusChanged( Kasten::AbstractView* )),
             SIGNAL(viewFocusChanged( Kasten::AbstractView* )) );
    connect( secondViewArea, SIGNAL(closeRequest( const QList<Kasten::AbstractView*>& )),
             SIGNAL(closeRequest( const QList<Kasten::AbstractView*>& )) );
    connect( secondViewArea, SIGNAL(removing( const QList<Kasten::AbstractView*>& )),
             SLOT(onViewsRemoved()) );
    mViewAreaList.append( secondViewArea );
    mCurrentViewArea = secondViewArea;

    splitter->setOrientation( orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal );
    splitter->addWidget( secondViewArea->widget() );
    // set to equal sizes
    QList<int> splitterSizes = splitter->sizes();
    // TODO: check if there are more, style-dependent spaces
    const int equalSize = ( splitterSizes[0] + splitterSizes[1] - splitter->handleWidth() ) / 2;
    splitterSizes[1] = splitterSizes[0] = equalSize;
    splitter->setSizes( splitterSizes );

    QList<AbstractViewArea*> viewAreas;
    viewAreas.append( secondViewArea );
    emit viewAreasAdded( viewAreas );
    emit viewAreaFocusChanged( secondViewArea );
}

void MultiViewAreas::closeViewArea( AbstractViewArea* _viewArea )
{
    TabbedViews* viewArea = static_cast<TabbedViews*>( _viewArea );

    const QList<AbstractView*> views = viewArea->viewList();

    emit closeRequest( views );
}

void MultiViewAreas::onViewsRemoved()
{
    // keep a minimum of one area
    if( mViewAreaList.count() < 2 )
        return;

    TabbedViews* viewArea = qobject_cast<TabbedViews *>( sender() );

    if( viewArea->viewCount() == 0 )
    {
        QWidget* viewAreaWidget = viewArea->widget();
        QSplitter* baseSplitter = static_cast<QSplitter*>( viewAreaWidget->parentWidget() );

        const int index = baseSplitter->indexOf( viewAreaWidget );
        const int otherIndex = 1 - index;

        QWidget* otherWidget = baseSplitter->widget( otherIndex );
        // do not delete the main splitter
        if( baseSplitter != mMainSplitter )
        {
            QSplitter* baseOfBaseSplitter = static_cast<QSplitter*>( baseSplitter->parentWidget() );

            const QList<int> baseOfBaseSplitterSizes = baseOfBaseSplitter->sizes();
            const int indexOfBaseSplitter = baseOfBaseSplitter->indexOf( baseSplitter );
            baseOfBaseSplitter->insertWidget( indexOfBaseSplitter, otherWidget );
            viewAreaWidget->setParent( 0 );
            delete baseSplitter;
            baseOfBaseSplitter->setSizes( baseOfBaseSplitterSizes );
        }

        mViewAreaList.removeOne( viewArea );
        if( mCurrentViewArea == viewArea )
        {
            foreach( TabbedViews* viewArea, mViewAreaList )
            {
                if( viewArea->widget() == otherWidget )
                {
                    mCurrentViewArea = viewArea;
//                     previousViewArea->setFocus();
                    break;
                }
            }
        }

        emit viewAreaFocusChanged( mCurrentViewArea );
        QList<AbstractViewArea*> viewAreas;
        viewAreas.append( viewArea );
        emit viewAreasRemoved( viewAreas );

        delete viewArea;
    }
}

void MultiViewAreas::onViewAreaFocusChanged( bool hasFocus )
{
    TabbedViews* viewArea = qobject_cast<TabbedViews *>( sender() );

    if( mCurrentViewArea == viewArea )
        return;

    // TODO: care for ! hasFocus?
    if( hasFocus )
    {
        mCurrentViewArea = viewArea;

        emit viewAreaFocusChanged( viewArea );
        emit viewFocusChanged( viewArea->viewFocus() );
    }
}

#if 0
void MultiViewAreas::onModifiedChanged( AbstractDocument::SyncStates newStates )
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

MultiViewAreas::~MultiViewAreas()
{
    qDeleteAll( mViewAreaList );
    delete mMainSplitter;
}

}
