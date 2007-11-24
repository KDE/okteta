/***************************************************************************
                          copyascontroller.cpp  -  description
                             -------------------
    begin                : Thu Nov 22 2007
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


#include "copyascontroller.h"

// kakao
#include <kabstractdocument.h>
#include <kabstractdocumentstreamencoder.h>
#include <kdocumentcodecmanager.h>
#include <kdocumentmanager.h>
#include <kabstractview.h>
#include <kidataselectable.h>
// KDE
#include <KXmlGuiWindow>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocale>
// Qt
#include <QtCore/QBuffer>
#include <QtCore/QMimeData>
#include <QtGui/QActionGroup>
#include <QtGui/QClipboard>
#include <QtGui/QApplication>

static const char CopyAsActionListId[] = "copy_as_list";

CopyAsController::CopyAsController( KDocumentManager *documentManager, KXmlGuiWindow *mainWindow )
 : mDocumentManager( documentManager ), mMainWindow( mainWindow )
{
    mCopyAsActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
//     mCopyAsActionGroup->setExclusive( true );
    connect( mCopyAsActionGroup, SIGNAL(triggered( QAction* )), SLOT(onActionTriggered( QAction* )) );

//     connect( mViewManager, SIGNAL(opened( KAbstractView* )),  SLOT(updateActions()) );
//     connect( mViewManager, SIGNAL(closing( KAbstractView* )), SLOT(updateActions()) );

    setView( 0 );
}

void CopyAsController::setView( KAbstractView *view )
{
Q_UNUSED(view)
    mView = view;
    updateActions();
}

 Q_DECLARE_METATYPE(KAbstractDocumentStreamEncoder*)

void CopyAsController::updateActions()
{
    mMainWindow->unplugActionList( CopyAsActionListId );

    qDeleteAll( mCopyAsActionGroup->actions() );

    const QList<KAbstractDocumentStreamEncoder*> encoderList =  mDocumentManager->codecManager()->encoderList();

    for( int c = 0; c < encoderList.size(); ++c )
    {
        KAbstractDocumentStreamEncoder *encoder = encoderList.at( c );
        const QString title = encoder->remoteTypeName();
        QAction *action = new QAction( title, mCopyAsActionGroup );
//         action->setCheckable( true );

        action->setData( QVariant::fromValue(encoder) );
        mCopyAsActionGroup->addAction( action );
    }
    mMainWindow->plugActionList( CopyAsActionListId, mCopyAsActionGroup->actions() );
}

void CopyAsController::onActionTriggered( QAction *action )
{
    KAbstractDocumentStreamEncoder *encoder = action->data().value<KAbstractDocumentStreamEncoder *>();

    KDE::If::DataSelectable *selectionControl = qobject_cast<KDE::If::DataSelectable *>( mView );
    const KAbstractDocumentSelection *selection = ( selectionControl ) ? selectionControl->selection() : 0;

    QByteArray exportData;
    QBuffer exportDataBuffer( &exportData );
    exportDataBuffer.open( QIODevice::WriteOnly );

    if( selection )
        encoder->encodeToStream( &exportDataBuffer, selection );
    else
        encoder->encodeToStream( &exportDataBuffer, mView->document() );
    exportDataBuffer.close();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData( encoder->remoteMimeType(), exportData );
    QApplication::clipboard()->setMimeData( mimeData, QClipboard::Clipboard );
}
