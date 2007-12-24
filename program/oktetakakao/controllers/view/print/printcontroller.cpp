/***************************************************************************
                          printcontroller.cpp  -  description
                             -------------------
    begin                : Thu Dec 6 2007
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


#include "printcontroller.h"

// controller
#include "printtool.h"
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
#include <KStandardAction>
#include <KAction>


PrintController::PrintController( KXmlGuiWindow *window )
 : mWindow( window ), mDocument( 0 ), mPrintTool( 0 )
{
    KActionCollection *actionCollection = mWindow->actionCollection();

    mPrintAction = KStandardAction::print( this, SLOT(print()), actionCollection );

    setView( 0 );
}

void PrintController::setView( KAbstractView *View )
{
    mDocument = View ? static_cast<KByteArrayDocument*>( View->document() ) : 0;

    const bool isDocument = ( mDocument != 0 );
    mPrintAction->setEnabled( isDocument );
}

void PrintController::print()
{
    // ensure tool
    if( !mPrintTool )
        mPrintTool = new PrintTool( mWindow );

    mPrintTool->print( mDocument );
}

PrintController::~PrintController()
{
    delete mPrintTool;
}

