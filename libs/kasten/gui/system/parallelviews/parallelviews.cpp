/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "parallelviews.h"

// lib
#include "parallelwidget.h"
#include <abstractview.h>
#include <abstractdocument.h>
#include <viewmanager.h>


ParallelViews::ParallelViews( ViewManager* viewManager )
 : mViewManager( viewManager )
{
    mParallelWidget = new ParallelWidget();

    connect( mParallelWidget, SIGNAL(currentChanged( QWidget* )), SLOT(onCurrentChanged( QWidget* )) );
}

QWidget* ParallelViews::widget() const { return mParallelWidget; }

void ParallelViews::addView( AbstractView* view )
{
    connect( view, SIGNAL(titleChanged( QString )), SLOT(onTitleChanged( QString )) );

    mParallelWidget->addWidget( view->widget(), view->title() );

    emit added( view );
}

void ParallelViews::removeView( AbstractView* view )
{
    view->disconnect( this );

    mParallelWidget->removeWidget( view->widget() );

    emit removing( view );
}

void ParallelViews::setViewFocus( AbstractView* view )
{
    mParallelWidget->setCurrentWidget( view->widget() );
}

AbstractView *ParallelViews::viewFocus() const
{
    return mViewManager->viewByWidget( mParallelWidget->currentWidget() );
}

void ParallelViews::onCurrentChanged( QWidget* widget )
{
    AbstractView* view = mViewManager->viewByWidget( widget );

    emit viewFocusChanged( view );
}

void ParallelViews::onTitleChanged( const QString& newTitle )
{
    AbstractView* view = qobject_cast<AbstractView *>( sender() );
    if( view )
        mParallelWidget->setTitle( view->widget(), newTitle );
}

#if 0
void ParallelViews::onModifiedChanged( AbstractDocument::SyncStates newStates )
{
Q_UNUSED( newStates )
    AbstractView* view = qobject_cast<AbstractView *>( sender() );
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

ParallelViews::~ParallelViews()
{
}
