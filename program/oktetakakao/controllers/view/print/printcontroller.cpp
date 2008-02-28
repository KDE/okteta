/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "printcontroller.h"

// controller
#include "printtool.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
// Kakao core
#include <kabstractdocument.h>
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

