/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "copyascontroller.h"

// Kakao gui
#include <kidataselectable.h>
#include <kabstractview.h>
// Kakao core
#include <kdocumentcodecmanager.h>
#include <kdocumentmanager.h>
#include <kabstractdocumentstreamencoder.h>
#include <kabstractdocument.h>
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

CopyAsController::CopyAsController( KDocumentManager *documentManager, KXmlGuiWindow *window )
 : mDocumentManager( documentManager ), mWindow( window ), mView( 0 )
{
    mCopyAsActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
    connect( mCopyAsActionGroup, SIGNAL(triggered( QAction* )), SLOT(onActionTriggered( QAction* )) );

    setView( 0 );
}

void CopyAsController::setView( KAbstractView *view )
{
    if( mView ) mView->disconnect( mCopyAsActionGroup );

    mView = view;
    mSelectionControl = view ? qobject_cast<KDE::If::DataSelectable *>( view ) : 0;

    if( mSelectionControl )
    {
        connect( mView, SIGNAL(hasSelectedDataChanged( bool )),
                 mCopyAsActionGroup, SLOT(setEnabled( bool )) );
    }

    updateActions();

    const bool hasSelectedData = ( mSelectionControl != 0 ) ? mSelectionControl->hasSelectedData() : false;
    mCopyAsActionGroup->setEnabled( hasSelectedData );
}

Q_DECLARE_METATYPE(KAbstractDocumentStreamEncoder*)

void CopyAsController::updateActions()
{
    mWindow->unplugActionList( CopyAsActionListId );

    qDeleteAll( mCopyAsActionGroup->actions() );

    const QList<KAbstractDocumentStreamEncoder*> encoderList =  mDocumentManager->codecManager()->encoderList();

    for( int c = 0; c < encoderList.size(); ++c )
    {
        KAbstractDocumentStreamEncoder *encoder = encoderList.at( c );
        const QString title = encoder->remoteTypeName();
        QAction *action = new QAction( title, mCopyAsActionGroup );

        action->setData( QVariant::fromValue(encoder) );
        mCopyAsActionGroup->addAction( action );
    }
    mWindow->plugActionList( CopyAsActionListId, mCopyAsActionGroup->actions() );
}

void CopyAsController::onActionTriggered( QAction *action )
{
    KAbstractDocumentStreamEncoder *encoder = action->data().value<KAbstractDocumentStreamEncoder *>();

    const KAbstractDocumentSelection *selection = mSelectionControl->selection();

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
