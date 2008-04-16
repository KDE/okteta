/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "filtercontroller.h"

// controller
#include "filterview.h"
#include "filtertool.h"
// filter
#include <filter/andbytearrayfilter.h>
#include <filter/orbytearrayfilter.h>
#include <filter/xorbytearrayfilter.h>
#include <filter/invertbytearrayfilter.h>
#include <filter/reversebytearrayfilter.h>
#include <filter/rotatebytearrayfilter.h>
#include <filter/shiftbytearrayfilter.h>
#include <filter/nobytearrayfilterparametersetedit.h>
#include <filter/operandbytearrayfilterparametersetedit.h>
#include <filter/reversebytearrayfilterparametersetedit.h>
#include <filter/rotatebytearrayfilterparametersetedit.h>
// KDE
#include <KActionCollection>
#include <KXmlGuiWindow>
#include <KLocale>
// Qt
#include <QtGui/QDockWidget>
#include <QtGui/QAction>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
FilterController::FilterController( KXmlGuiWindow *window )
 : mTool( new FilterTool() )
{
// move to view
    KActionCollection *actionCollection = window->actionCollection();

    mView = new FilterView( mTool );
    loadFilter();

    QDockWidget *dockWidget = new QDockWidget( i18nc("@title:window", "Binary Filter"), window );
    dockWidget->setObjectName( "BinaryFilter" );
    dockWidget->setWidget( mView );
    window->addDockWidget( Qt::RightDockWidgetArea, dockWidget );

    QAction *action = dockWidget->toggleViewAction();
    action->setText( i18nc("@title:window", "Binary Filter") ); //TODO: better name needed!
    actionCollection->addAction( "show_binary_filter", action );

    setView( 0 );
}

void FilterController::setView( KAbstractView *view )
{
    mTool->setView( view );
}

void FilterController::loadFilter()
{
    AbstractByteArrayFilter *filter;

    filter = new AndByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new OperandByteArrayFilterParameterSetEdit() );
    filter = new OrByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new OperandByteArrayFilterParameterSetEdit() );
    filter = new XOrByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new OperandByteArrayFilterParameterSetEdit() );
    filter = new InvertByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new NoByteArrayFilterParameterSetEdit() );
    filter = new ReverseByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new ReverseByteArrayFilterParameterSetEdit() );
    filter = new RotateByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new RotateByteArrayFilterParameterSetEdit() );
    filter = new ShiftByteArrayFilter();
    mTool->addFilter( filter );
    mView->addParameterEdit( filter->name(), new RotateByteArrayFilterParameterSetEdit() );
}


FilterController::~FilterController()
{
    delete mTool;
}
