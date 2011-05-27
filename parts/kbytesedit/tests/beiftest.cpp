/*
    This file is part of the Okteta KByteArrayEdit module, made within the KDE community.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// KDE
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


  KCmdLineArgs::init( argc, argv, "Example", 0,KLocalizedString(),"" );
  KApplication app;

  QWidget *BytesEditWidget = createBytesEditWidget();
  // was kdesdk installed, so the widget could be found?
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
      CharColumn->setSubstituteChar( QLatin1Char('*') );
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
