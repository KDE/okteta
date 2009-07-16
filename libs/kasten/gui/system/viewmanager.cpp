/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewmanager.h"

// lib
#include "abstractviewfactory.h"
#include "dummyview.h"
#include "documentmanager.h"
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QListIterator>
#include <QtCore/QMutableListIterator>

// temporary
#include "modelcodecviewmanager.h"


namespace Kasten
{

ViewManager::ViewManager( DocumentManager* documentManager )
 : mDocumentManager( documentManager ),
   mCodecViewManager( new ModelCodecViewManager() )
{
    connect( mDocumentManager, SIGNAL(added( Kasten::AbstractDocument* )), SLOT(createViewFor( Kasten::AbstractDocument* )) );
    connect( mDocumentManager, SIGNAL(closing( Kasten::AbstractDocument* )), SLOT(removeViewsFor( Kasten::AbstractDocument* )) );
}

void ViewManager::setWindow( KXmlGuiWindow *window )
{
    mMainWindow = window;
}

void ViewManager::setViewFactory( AbstractViewFactory* factory )
{
    mFactory = factory;
}

QList<AbstractView*> ViewManager::views() const
{
    return mViewList;
}

AbstractView* ViewManager::viewByWidget( QWidget* widget ) const
{
    AbstractView* result = 0;

    QListIterator<AbstractView*> it( mViewList );
    while( it.hasNext() )
    {
        AbstractView* view = it.next();
        if( view->widget() == widget)
        {
            result = view;
            break;
        }
    }
    return result;
}


void ViewManager::createViewFor( AbstractDocument* document )
{
    if( !document )
        return;

    AbstractView* view = mFactory->createViewFor( document );
    if( !view )
        view = new DummyView( document );

    mViewList.append( view );
    emit opened( view );
}


void ViewManager::removeViewsFor( AbstractDocument* document )
{
    QMutableListIterator<AbstractView*> it( mViewList );
    while( it.hasNext() )
    {
        AbstractView* view = it.next();
        if( view->document() == document )
        {
            it.remove();
            emit closing( view );
            delete view;
        }
    }
}

AbstractView* ViewManager::viewOfDocument( AbstractDocument* document ) const
{
    AbstractView* result = 0;
    foreach( AbstractView* view, mViewList )
    {
        if( view->document() == document )
        {
            result = view;
            break;
        }
    }
    return result;
}


ViewManager::~ViewManager()
{
    delete mCodecViewManager;
    delete mFactory;
}

}
