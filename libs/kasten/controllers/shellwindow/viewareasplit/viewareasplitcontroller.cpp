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

#include "viewareasplitcontroller.h"

// lib
#include <abstractgroupedviews.h>
#include <viewareasplitable.h>
#include <abstractviewarea.h>
#include <viewfocusable.h>
#include <viewmanager.h>
// KDE
#include <KXMLGUIClient>
#include <KActionCollection>
#include <KAction>
#include <KLocale>


namespace Kasten
{

ViewAreaSplitController::ViewAreaSplitController( ViewManager* viewManager, AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient )
  : mViewManager( viewManager ),
    mGroupedViews( groupedViews ),
    mViewAreaSplitable( 0 ),
    mCurrentViewArea( 0 )
{
    mViewAreaSplitable = mGroupedViews ? qobject_cast<If::ViewAreaSplitable*>( mGroupedViews ) : 0;
    if( mViewAreaSplitable )
    {
        connect( mGroupedViews, SIGNAL(viewAreaFocusChanged( Kasten::AbstractViewArea* )),
                 SLOT(onViewAreaFocusChanged( Kasten::AbstractViewArea* )) );
        connect( mGroupedViews, SIGNAL(viewAreasAdded( const QList<Kasten::AbstractViewArea*>& )),
                 SLOT(onViewAreasChanged()) );
        connect( mGroupedViews, SIGNAL(viewAreasRemoved( const QList<Kasten::AbstractViewArea*>& )),
                 SLOT(onViewAreasChanged()) );
    }

    KActionCollection* actionCollection = guiClient->actionCollection();

    mSplitVerticallyAction = actionCollection->addAction( "view_area_split_vertically" );
    mSplitVerticallyAction->setText( i18nc("@title:menu","Split Vertically") );
    mSplitVerticallyAction->setIcon( KIcon("view-split-left-right") );
    mSplitVerticallyAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_L );
    mSplitVerticallyAction->setEnabled( mViewAreaSplitable != 0 );
    connect( mSplitVerticallyAction, SIGNAL(triggered()), SLOT(splitVertically()) );

    mSplitHorizontallyAction = actionCollection->addAction( "view_area_split_horizontally" );
    mSplitHorizontallyAction->setText( i18nc("@title:menu","Split Horizontal") );
    mSplitHorizontallyAction->setIcon( KIcon("view-split-top-bottom") );
    mSplitHorizontallyAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_T );
    mSplitHorizontallyAction->setEnabled( mViewAreaSplitable != 0 );
    connect( mSplitHorizontallyAction, SIGNAL(triggered()), SLOT(splitHorizontally()) );

    mCloseAction = actionCollection->addAction( "view_area_close" );
    mCloseAction->setText( i18nc("@title:menu","Close View Area") );
    mCloseAction->setIcon( KIcon("view-close") );
    mCloseAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_R );
    connect( mCloseAction, SIGNAL(triggered()), SLOT(close()) );

    onViewAreaFocusChanged( mViewAreaSplitable ? mViewAreaSplitable->viewAreaFocus() : 0 );
    onViewAreasChanged();
}

void ViewAreaSplitController::setTargetModel( AbstractModel* model )
{
Q_UNUSED(model)
}

void ViewAreaSplitController::splitVertically()
{
    splitViewArea( Qt::Vertical );
}

void ViewAreaSplitController::splitHorizontally()
{
    splitViewArea( Qt::Horizontal );
}

void ViewAreaSplitController::close()
{
    mViewAreaSplitable->closeViewArea( mCurrentViewArea );
}

void ViewAreaSplitController::splitViewArea( Qt::Orientation orientation )
{
    If::ViewFocusable* viewFocusable = qobject_cast<If::ViewFocusable*>( mCurrentViewArea );

    if( ! viewFocusable )
        return;

    AbstractView* currentView = viewFocusable->viewFocus();

    mViewAreaSplitable->splitViewArea( mCurrentViewArea, orientation );

    // TODO: ideal would be a new view which copies the existing one
    // and starts visually where the old one stops after the resize
    const Qt::Alignment alignment = ( orientation == Qt::Horizontal ) ? Qt::AlignBottom : Qt::AlignRight;
    mViewManager->createCopyOfView( currentView, alignment );
}

void ViewAreaSplitController::onViewAreaFocusChanged( AbstractViewArea* viewArea )
{
    if( mCurrentViewArea )
        mCurrentViewArea->disconnect( this );

    // TODO: how to handle single view areas? examples, signals?
    mCurrentViewArea = qobject_cast<AbstractGroupedViews*>( viewArea );

    if( mCurrentViewArea )
    {
        connect( mCurrentViewArea, SIGNAL(added( const QList<Kasten::AbstractView*>& )),
                 SLOT(onViewsChanged()) );
        connect( mCurrentViewArea, SIGNAL(removing( const QList<Kasten::AbstractView*>& )),
                 SLOT(onViewsChanged()) );
    }
    onViewsChanged();
}

void ViewAreaSplitController::onViewAreasChanged()
{
    const bool hasMultipleViewArea = mViewAreaSplitable ? ( mViewAreaSplitable->viewAreasCount() > 1 ) : false;

    mCloseAction->setEnabled( hasMultipleViewArea );
}

void ViewAreaSplitController::onViewsChanged()
{
    const bool hasViews = mCurrentViewArea ? ( mCurrentViewArea->viewCount() > 0 ) : false;

    mSplitVerticallyAction->setEnabled( hasViews );
    mSplitHorizontallyAction->setEnabled( hasViews );
}

}
