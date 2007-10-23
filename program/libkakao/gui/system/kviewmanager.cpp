/***************************************************************************
                          kviewmanager.cpp  -  description
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


KViewManager::KViewManager( KDocumentManager *documentManager )
 : mDocumentManager( documentManager )
{
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


void KViewManager::closeView( KAbstractView* view )
{
    QMutableListIterator<KAbstractView*> searchIterator( mViewList );
    if( searchIterator.findNext(view) )
    {
        // only view to document?
        // TODO: loop could be integrated in previous loop searchIterator.findNext
        KAbstractDocument *document = view->document();

        bool documentHasOtherView = false;
        QListIterator<KAbstractView*> it( mViewList );
        while( it.hasNext() )
        {
            const KAbstractView *otherView = it.next();
            if( otherView->document() == document && otherView != view )
            {
                documentHasOtherView = true;
                break;
            }
        }

        if( documentHasOtherView || mDocumentManager->canClose(document) )
        {

            searchIterator.remove();
            emit closing( view );
            delete view;
        }
    }
}


KViewManager::~KViewManager()
{
    delete mFactory;
}

#include "kviewmanager.moc"
