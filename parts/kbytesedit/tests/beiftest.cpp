/***************************************************************************
                          beiftest.cpp  -  description
                             -------------------
    begin                : Tue May 16 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// kde specific
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <khexedit/byteseditinterface.h>
#include <khexedit/valuecolumninterface.h>
#include <khexedit/charcolumninterface.h>


using namespace KHE;

static int BufferSize = 99;
int main( int argc, char* argv[] )
{
  char *Buffer = new char[BufferSize];
  memset( Buffer, '#', BufferSize );
  memcpy( &Buffer[5], "This is a test text.", 21 );
  memcpy( &Buffer[23], "This is a test text.", 21 );


  KCmdLineArgs::init( argc, argv, "Example","","","" );
  KApplication app;

  QWidget *BytesEditWidget = createBytesEditWidget();
  // was kdeutils installed, so the widget could be found?
  if( BytesEditWidget )
  {
    // fetch the editor interface
    BytesEditInterface *BytesEdit = bytesEditInterface( BytesEditWidget );
    Q_ASSERT( BytesEdit ); // This should not fail!

    // now use the editor.
//     BytesEdit->setData( Buffer, BufferSize );
    BytesEdit->setMaxDataSize( BufferSize );
    BytesEdit->setReadOnly( false );

    ValueColumnInterface *ValueColumn = valueColumnInterface( BytesEditWidget );
    if( ValueColumn )
    {
//     ValueColumn->setCoding( ValueColumnInterface::DecimalCoding );
      ValueColumn->setCoding( ValueColumnInterface::BinaryCoding );
      ValueColumn->setByteSpacingWidth( 2 );
      ValueColumn->setNoOfGroupedBytes( 3 );
      ValueColumn->setGroupSpacingWidth( 4 );
    }

    CharColumnInterface *CharColumn = charColumnInterface( BytesEditWidget );
    if( CharColumn )
    {
      CharColumn->setShowUnprintable( false );
      CharColumn->setSubstituteChar( '*' );
    }

    // Finally insert the widget into the layout of its parent
    app.setTopWidget( BytesEditWidget );
    BytesEditWidget->show();
  }
  else {
    // Don't offer the editor widget.
  }

  app.exec();
  delete [] Buffer;
}
