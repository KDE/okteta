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

#include "stringsextractcontroller.h"

// controller
#include "stringsextractdialog.h"
#include "stringsextracttool.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
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
    if( mViewWidget ) mViewWidget->disconnect( this );

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

