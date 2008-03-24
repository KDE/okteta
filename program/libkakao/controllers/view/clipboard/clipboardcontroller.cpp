/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "clipboardcontroller.h"

// Kakao gui
#include <kidataselectable.h>
#include <kiselecteddatawriteable.h>
#include <kabstractview.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QtGui/QClipboard>
#include <QtGui/QApplication>


ClipboardController::ClipboardController( KXmlGuiWindow *window )
 : mMainWindow( window ), mView( 0 ), mSelectionControl( 0 ), mMimeDataControl( 0 )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    mCutAction =   KStandardAction::cut(   this, SLOT(cut()),   actionCollection );
    mCopyAction =  KStandardAction::copy(  this, SLOT(copy()),  actionCollection );
    mPasteAction = KStandardAction::paste( this, SLOT(paste()), actionCollection );

    connect( QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(onClipboardDataChanged()) );

    setView( 0 );
}

void ClipboardController::setView( KAbstractView *view )
{
    if( mView ) mView->disconnect( this );

    mView = view;
    mSelectionControl = view ? qobject_cast<KDE::If::DataSelectable *>( view ) : 0;

    if( mSelectionControl )
    {
        connect( mView, SIGNAL(hasSelectedDataChanged( bool )), SLOT(onHasSelectedDataChanged( bool )) );

        mMimeDataControl = view ? qobject_cast<KDE::If::SelectedDataWriteable*>( view ) : 0;
        if( mMimeDataControl )
        {
           // TODO: catch 
//         if( !isReadOnly() && isOverWrite() )
//         connect( mView, SIGNAL(hasSelectedDataChanged( bool )), SLOT(onHasSelectedDataChanged( bool )) );
//         connect( HexEdit, SIGNAL(copyAvailable(bool)), mCopyAction,SLOT(setEnabled(bool)) );
//         mCopyAction->setEnabled( false );
        }
    }
    else
        mMimeDataControl = 0;

    const bool hasSelectedData = ( mSelectionControl != 0 ) ? mSelectionControl->hasSelectedData() : false;
    const bool isWriteable = ( mMimeDataControl != 0 );

    mCopyAction->setEnabled( hasSelectedData );
    mCutAction->setEnabled( hasSelectedData && isWriteable );//&& (!isReadOnly() && !isOverWrite()) );
    onClipboardDataChanged();
}


void ClipboardController::onHasSelectedDataChanged( bool hasSelectedData )
{
    const bool isWriteable = ( mMimeDataControl != 0 );

    mCopyAction->setEnabled( hasSelectedData );
    mCutAction->setEnabled( hasSelectedData && isWriteable );//&& (!isReadOnly() && !isOverWrite()) );
}

void ClipboardController::onClipboardDataChanged()
{
    const bool isWriteable = ( mMimeDataControl != 0 ) ?
        mMimeDataControl->canReadData(QApplication::clipboard()->mimeData()) : false;
    mPasteAction->setEnabled( isWriteable );//&& !isReadOnly() );
}

void ClipboardController::cut()
{
    QMimeData *data = mMimeDataControl->cutSelectedData();
    if( !data )
      return;

    QApplication::clipboard()->setMimeData( data, QClipboard::Clipboard );
}


void ClipboardController::copy()
{
    QMimeData *data = mSelectionControl->copySelectedData();
    if( !data )
      return;

    QApplication::clipboard()->setMimeData( data, QClipboard::Clipboard );
}


void ClipboardController::paste()
{
    const QMimeData *data = QApplication::clipboard()->mimeData( QClipboard::Clipboard );
    mMimeDataControl->insertData( data );
}
