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
#include "insertpatterndialog.h"
#include "insertpatterntool.h"
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
 : mWindow( window ),
   mInsertPatternTool( new InsertPatternTool() ), mInsertPatternDialog( 0 )
{
    connect( mInsertPatternTool, SIGNAL(viewChanged( bool )), SLOT(onViewChanged( bool )) );

    KActionCollection *actionCollection = mWindow->actionCollection();

    mInsertPatternAction = actionCollection->addAction( "insert_pattern" );
    mInsertPatternAction->setText( i18n("&Insert pattern...") );
    connect( mInsertPatternAction, SIGNAL(triggered(bool) ), SLOT(onActionTriggered()));
    mInsertPatternAction->setShortcut( Qt::CTRL + Qt::Key_Insert );

    setView( 0 );
}

void InsertPatternController::setView( KAbstractView *view )
{
    mInsertPatternTool->setView( view );
}

void InsertPatternController::onViewChanged( bool hasView )
{
    if( !hasView && mInsertPatternDialog )
        mInsertPatternDialog->hide();

    mInsertPatternAction->setEnabled( hasView );
}


void InsertPatternController::onActionTriggered()
{
    // ensure dialog
    if( !mInsertPatternDialog )
    {
        mInsertPatternDialog = new InsertPatternDialog( mWindow );
        connect( mInsertPatternDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }
    mInsertPatternDialog->setCharCode( mInsertPatternTool->charCodecName() );

    mInsertPatternDialog->show();
}

void InsertPatternController::onOkClicked()
{
    mInsertPatternDialog->hide();

    const QByteArray pattern = mInsertPatternDialog->pattern();
    const int patternNumber = mInsertPatternDialog->number();

    mInsertPatternTool->insertPattern( pattern, patternNumber );
}

InsertPatternController::~InsertPatternController()
{
    delete mInsertPatternDialog;
    delete mInsertPatternTool;
}
