/***************************************************************************
                          stringsextractcontroller.cpp  -  description
                             -------------------
    begin                : Wed Dec 26 2007
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


#include "stringsextractcontroller.h"

// controller
#include "stringsextractdialog.h"
#include "stringsextracttool.h"
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
#include <KXmlGuiWindow>
#include <KActionCollection>
#include <KLocale>
#include <KAction>
// Qt
#include <QtGui/QAction>


StringsExtractController::StringsExtractController( KXmlGuiWindow *window )
 : mWindow( window ),
   mViewWidget( 0 ), mDocument( 0 ),
   mStringsExtractTool( 0 ), mStringsExtractDialog( 0 )
{
    KActionCollection *actionCollection = mWindow->actionCollection();

    mStringsExtractAction = actionCollection->addAction( "extract_strings" );
    mStringsExtractAction->setText( i18n("&Extract strings...") );
    connect( mStringsExtractAction, SIGNAL(triggered(bool) ), SLOT(showTool()));

    setView( 0 );
}

// TODO: tools should get the setView, and the controller should get informed
void StringsExtractController::setView( KAbstractView *view )
{
    mViewWidget->disconnect( this );

    mViewWidget = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    mDocument = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;

    const bool isDocument = ( mDocument != 0 );
    mStringsExtractAction->setEnabled( isDocument );

    if( mStringsExtractTool )
    {
        mStringsExtractTool->setDocument( mDocument );
        mStringsExtractTool->setCharCodec( mViewWidget->encodingName() );
        connect( mViewWidget, SIGNAL(charCodecChanged( const QString & )), SLOT(onCharCodecChange( const QString &)) );
    }
}

void StringsExtractController::showTool()
{
    // ensure tool
    if( !mStringsExtractTool )
    {
        mStringsExtractTool = new StringsExtractTool();

        mStringsExtractTool->setDocument( mDocument );
        mStringsExtractTool->setCharCodec( mViewWidget->encodingName() );

        mStringsExtractDialog = new StringsExtractDialog( mStringsExtractTool, mWindow );

        mStringsExtractTool->extract();
        connect( mViewWidget, SIGNAL(charCodecChanged( const QString & )), SLOT(onCharCodecChange( const QString &)) );
    }
    mStringsExtractDialog->show();
}

void StringsExtractController::onCharCodecChange( const QString &codeName )
{
    mStringsExtractTool->setCharCodec( codeName );
}

StringsExtractController::~StringsExtractController()
{
    delete mStringsExtractDialog;
    delete mStringsExtractTool;
}

