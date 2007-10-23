/***************************************************************************
                          poddecodercontroller.cpp  -  description
                             -------------------
    begin                : Fri Nov 3 2006
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


#include "poddecodercontroller.h"

// controller
#include "kprimitivetypesview.h"
//
#include <kbytearraydocument.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// kakao
#include <kviewmanager.h>
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtGui/QDockWidget>


PODDecoderController::PODDecoderController( KViewManager */*ViewManager*/, KXmlGuiWindow *MW )
 : MainWindow( MW ), HexEdit( 0 ), ByteArray( 0 ), CursorIndex( 0 )
{
    QDockWidget *DW = new QDockWidget( MainWindow );
    PrimitiveTypesView = new KPrimitiveTypesView( DW );
    DW->setWidget( PrimitiveTypesView );
    MainWindow->addDockWidget( Qt::BottomDockWidgetArea, DW );
    //Window->addToolWidget( PrimitiveTypesView );

    // force unset
    update();
}

void PODDecoderController::setView( KAbstractView *View )
{
    disconnect( HexEdit );
    disconnect( ByteArray );

    HexEdit = View ? static_cast<KHEUI::KByteArrayView *>( View->widget() ) : 0;
    KByteArrayDocument *Document = View ? static_cast<KByteArrayDocument*>( View->document() ) : 0;
    ByteArray = Document ? Document->content() : 0;

    if( ByteArray && HexEdit )
    {
        CursorIndex = HexEdit->cursorPosition();
        connect( HexEdit, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChange( int )) );
        connect( ByteArray, SIGNAL(contentsChanged( int, int )), SLOT(onContentsChange( int, int )) );
        PrimitiveTypesView->setCharCode( HexEdit->encodingName() );
        PrimitiveTypesView->setUndefinedChar( HexEdit->undefinedChar() ); // TODO: track config changes in HexEdit
    }
    // force painting
    update();
}
#include <kdebug.h>

void PODDecoderController::onCursorPositionChange( int Pos )
{
// kDebug() << "onCursorPositionChange "<< Pos << endl;
    CursorIndex = Pos;
    update();
}

void PODDecoderController::onContentsChange( int Start, int End )
{
// kDebug() << "onContentsChange "<< Start <<" " <<End<< endl;
    if( Start-DecodeDataSize < CursorIndex && CursorIndex <= End )
        update();
}

// TODO: support display of chunks shorter than 4
void PODDecoderController::update()
{
    unsigned char *CurrentData;

    // interpreted range out of bounds?
    if( !ByteArray || CursorIndex+DecodeDataSize > ByteArray->size() )
        CurrentData = 0;
    else
    {
        ByteArray->copyTo( (char*)Data, CursorIndex, DecodeDataSize );
        CurrentData = Data;
    }

    PrimitiveTypesView->onByteArrayChange( CurrentData );
}
