#include <iostream>

#include <kapplication.h>
#include <kcmdlineargs.h>

#include <kparts/componentfactory.h>
// app
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
  KApplication app( argc, argv );

  QWidget *BytesEditWidget = createBytesEditWidget();
  // was kdeutils installed, so the widget could be found?
  if( BytesEditWidget )
  {
    // fetch the editor interface
//     KBytesEditInterface *BytesEdit = static_cast<KBytesEditInterface *>( BytesEditWidget->qt_cast( "KBytesEditInterface" ) );
    BytesEditInterface *BytesEdit = bytesEditInterface( BytesEditWidget );
    Q_ASSERT( BytesEdit ); // This should not fail!

    // now use the editor.
//     BytesEdit->setData( Buffer, BufferSize );
    BytesEdit->setMaxDataSize( BufferSize );
    BytesEdit->setReadOnly( false );
#if 0
    KHE::ValueColumnInterface *ValueColumn = hexColumnInterface( BytesEditWidget );
    if( ValueColumn )
    {
      Layout->setResizeStyle( KBytesEditInterface::LockGrouping );
//     BytesEdit->setNoOfBytesPerLine( 16 );
    }
#endif
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
