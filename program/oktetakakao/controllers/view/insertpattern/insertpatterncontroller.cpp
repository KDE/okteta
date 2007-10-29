/***************************************************************************
                          insertpatterncontroller.cpp  -  description
                             -------------------
    begin                : Thu Nov 23 2006
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


#include "insertpatterncontroller.h"

// controller
#include "kinsertpatterndialog.h"
//
#include <kbytearraydocument.h>
// kakao
#include <kviewmanager.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KActionCollection>
#include <KXmlGuiWindow>
#include <KLocale>
#include <KMessageBox>
#include <KAction>
// Qt
#include <QtGui/QAction>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
InsertPatternController::InsertPatternController( KXmlGuiWindow *window )
 : mMainWindow( window ), mHexEdit( 0 ), mByteArray( 0 ), mInsertPatternDialog( 0 )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    mInsertPatternAction = actionCollection->addAction( "insert_pattern" );
    mInsertPatternAction->setText( i18n("&Insert pattern...") );
    connect( mInsertPatternAction, SIGNAL(triggered(bool) ), SLOT(insertPattern()));
    mInsertPatternAction->setShortcut( Qt::CTRL + Qt::Key_Insert );

    setView( 0 );
}

void InsertPatternController::setView( KAbstractView *view )
{
    disconnect( mHexEdit );

    mHexEdit = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArray = document ? document->content() : 0;

    if( mByteArray )
    {
//         connect( mHexEdit, SIGNAL( selectionChanged( bool )), SLOT( onSelectionChanged( bool )) );
    }
    const bool hasView = ( mByteArray != 0 );
    mInsertPatternAction->setEnabled( hasView );
}


void InsertPatternController::insertPattern()
{
    // ensure dialog
    if( !mInsertPatternDialog )
    {
        mInsertPatternDialog = new KInsertPatternDialog( mMainWindow );
        connect( mInsertPatternDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }
    mInsertPatternDialog->setCharCode( mHexEdit->encodingName() );

    mInsertPatternDialog->show();
}

void InsertPatternController::onOkClicked()
{
    mInsertPatternDialog->hide();

    const QByteArray pattern = mInsertPatternDialog->pattern();
    const int patternNumber = mInsertPatternDialog->number();
    const int patternSize = pattern.size();

    const int insertDataSize = patternNumber * patternSize;
    QByteArray insertData( insertDataSize, '\0' );

    for( int i=0; i < insertDataSize; i+= patternSize )
        memcpy( &insertData.data()[i], pattern.constData(), patternSize );

    //TODO: support insert to selection
    mHexEdit->insert( insertData );
}

InsertPatternController::~InsertPatternController() {}
