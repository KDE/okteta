#include <iostream>

#include <kapplication.h>
#include <kcmdlineargs.h>

#include <kparts/componentfactory.h>
#include "../../interface/kbyteseditinterface.h"


static int BufferSize = 79;
int main( int argc, char* argv[] )
{
  char *Buffer = new char[BufferSize];
  memset( Buffer, '#', BufferSize );
  memcpy( &Buffer[5], "This is a test text.", 21 );
  memcpy( &Buffer[23], "This is a test text.", 21 );


  KCmdLineArgs::init( argc, argv, "Example","","" );
  KApplication app( argc, argv );

  QWidget *BytesEditWidget =
    KParts::ComponentFactory::createInstanceFromQuery<QWidget>("KHexEdit/KBytesEdit", QString::null, 0 );
  // was kdeutils installed, so the widget could be found?
  if( BytesEditWidget )
  {
    // fetch the editor interface
    KBytesEditInterface *BytesEdit = static_cast<KBytesEditInterface *>( BytesEditWidget->qt_cast( "KBytesEditInterface" ) );
    Q_ASSERT( BytesEdit ); // This should not fail!

    // now use the editor.
    BytesEdit->setData(Buffer, BufferSize );
    BytesEdit->setResizeStyle( KBytesEditInterface::LockGrouping );
    BytesEdit->setCoding( KBytesEditInterface::DecimalCoding );
    BytesEdit->setMaxDataSize( BufferSize );
    BytesEdit->setReadOnly( false );
    BytesEdit->setByteSpacingWidth( 2 );
    BytesEdit->setNoOfGroupedBytes( 3 );
    BytesEdit->setGroupSpacingWidth( 4 );
//     BytesEdit->setNoOfBytesPerLine( 16 );

    std::cout << "BSW: "<<BytesEdit->byteSpacingWidth() << std::endl;
    std::cout << "Data: "<<&BytesEdit->data()[5] << std::endl;
//     std::cout << "BSW: "<<BytesEdit->byteSpacingWidth() << std::endl;

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
