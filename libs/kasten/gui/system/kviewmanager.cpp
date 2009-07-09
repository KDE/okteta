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

#include "kviewmanager.h"

// lib
#include "kviewfactory.h"
#include "dummyview.h"
#include "kdocumentmanager.h"
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QListIterator>
#include <QtCore/QMutableListIterator>

// temporary
#include "modelcodecviewmanager.h"


namespace Kasten
{

KViewManager::KViewManager( KDocumentManager *documentManager )
 : mDocumentManager( documentManager ),
   mCodecViewManager( new ModelCodecViewManager() )
{
    connect( mDocumentManager, SIGNAL(added( KAbstractDocument* )), SLOT(createViewFor( KAbstractDocument* )) );
    connect( mDocumentManager, SIGNAL(closing( KAbstractDocument* )), SLOT(removeViewsFor( KAbstractDocument* )) );
}

void KViewManager::setWindow( KXmlGuiWindow *window )
{
    mMainWindow = window;
}

void KViewManager::setViewFactory( KViewFactory *factory )
{
    mFactory = factory;
}

QList<KAbstractView*> KViewManager::views() const
{
    return mViewList;
}

KAbstractView *KViewManager::viewByWidget( QWidget *widget ) const
{
    KAbstractView *result = 0;

    QListIterator<KAbstractView*> it( mViewList );
    while( it.hasNext() )
    {
        KAbstractView *view = it.next();
        if( view->widget() == widget)
        {
            result = view;
            break;
        }
    }
    return result;
}


void KViewManager::createViewFor( KAbstractDocument *document )
{
    if( !document )
        return;

    KAbstractView *view = mFactory->createViewFor( document );
    if( !view )
        view = new DummyView( document );

    mViewList.append( view );
    emit opened( view );
}


void KViewManager::removeViewsFor( KAbstractDocument *document )
{
    QMutableListIterator<KAbstractView*> it( mViewList );
    while( it.hasNext() )
    {
        KAbstractView *view = it.next();
        if( view->document() == document )
        {
            it.remove();
            emit closing( view );
            delete view;
        }
    }
}

KAbstractView* KViewManager::viewOfDocument( KAbstractDocument* document ) const
{
    KAbstractView* result = 0;
    foreach( KAbstractView* view, mViewList )
    {
        if( view->document() == document )
        {
            result = view;
            break;
        }
    }
    return result;
}


KViewManager::~KViewManager()
{
    delete mCodecViewManager;
    delete mFactory;
}

}
