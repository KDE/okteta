/***************************************************************************
                          clipboardcontroller.cpp  -  description
                             -------------------
    begin                : Mon Nov 13 2006
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


#include "clipboardcontroller.h"

//
#include <kidataselectable.h>
#include <kiselecteddatawriteable.h>
// kakao
#include <kviewmanager.h>
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

    setView( 0 );
}

void ClipboardController::setView( KAbstractView *view )
{
    mView->disconnect( this );

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

    const bool hasSelectedData = ( mSelectionControl != 0 ) ? mSelectionControl->hasSelectedData() : false;
    const bool isWriteable = ( mMimeDataControl != 0 );

    mCopyAction->setEnabled( hasSelectedData );
    mCutAction->setEnabled( hasSelectedData && isWriteable );//&& (!isReadOnly() && !isOverWrite()) );
    mPasteAction->setEnabled( isWriteable );//&& !isReadOnly() );
}


void ClipboardController::onHasSelectedDataChanged( bool hasSelectedData )
{
     const bool isWriteable = ( mMimeDataControl != 0 );

    mCopyAction->setEnabled( hasSelectedData );
    mCutAction->setEnabled( hasSelectedData && isWriteable );//&& (!isReadOnly() && !isOverWrite()) );
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
