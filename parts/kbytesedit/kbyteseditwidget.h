/***************************************************************************
                          kbyteseditwidget.h  -  description
                             -------------------
    begin                : Fri Aug 29 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KBYTESEDITWIDGET_H
#define KBYTESEDITWIDGET_H

#include <qwidget.h>
#include <kbyteseditinterface.h>

namespace KHE {
class KBytesEdit;
}

/**
   Simple hex editor/viewer.

   @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
   @version 0.1
 **/
class KBytesEditWidget : public QWidget, public KBytesEditInterface
{
  Q_OBJECT
  public:
    KBytesEditWidget( QWidget *parent, const char *name, const QStringList & = QStringList() );

  signals:

  public slots:

  public: // set methods
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param S size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is the maximum size)
      */
    virtual void setData( char *D, int S, int RS = -1, bool KM = true );
    /** sets whether the given array should be handled read only or not */
    virtual void setReadOnly( bool RO = true );
    /** sets the maximal size of the actual byte array. If the actual array is already larger
      * it will not be modified but there can be only done non-inserting actions
      * until the array's is below the limit
      */
    virtual void setMaxDataSize( int MS );
    /** sets whether the array should be deleted on the widget's end or if a new array is set.
      * Default is false
      */
    virtual void setAutoDelete( bool AD = true );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    virtual void setKeepsMemory( bool KM = true );
    /** */
    virtual void setCursorPosition( int Index );
    /** sets the format of the hex column. Default is KHE::HexadecimalCoding */
    virtual void setCoding( KCoding C );
    /** sets the resizestyle for the hex column. Default is KHE::FullSizeUsage */
    virtual void setResizeStyle( KResizeStyle Style );
    /** sets whether the widget is overwriteonly or not. Default is false. */
    virtual void setOverwriteOnly( bool b );
    /** sets whether the widget is in overwrite mode or not. Default is true. */
    virtual void setOverwriteMode( bool b );
    /** sets whether the data should be treated modified or not */
    virtual void setModified( bool b );
    /** sets the number of bytes per line, switching the resize style to KHE::NoResize */
    virtual void setNoOfBytesPerLine( int NoCpL );
    /** sets the spacing between the bytes in pixels */
    virtual void setByteSpacingWidth( int BSW );
    /** sets the numbers of grouped bytes, 0 means no grouping, Default is 4 */
    virtual void setNoOfGroupedBytes( int NoGB );
    /** sets the spacing between the groups in pixels */
    virtual void setGroupSpacingWidth( int GSW );
    /** sets the spacing in the middle of a binary byte in the hex column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      */
    virtual void setBinaryGapWidth( int BGW );


  public: // get methods
    virtual char *data() const;
    /** returns the size of the actual byte array */
    virtual int dataSize() const;
    /** returns the maximal allowed size for the byte array */
    virtual int maxDataSize () const;
    /** returns whether autodelete is set for the byte array */
    virtual bool isAutoDelete() const;
    /** returns whether the memory of the byte array is kept */
    virtual bool keepsMemory() const;

    /** */
    virtual bool isOverwriteMode() const;
    virtual bool isOverwriteOnly() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

//     virtual bool tabChangesFocus() const;

    virtual int noOfBytesPerLine() const;
    virtual KResizeStyle resizeStyle() const;
    virtual KCoding coding() const;
    virtual int byteSpacingWidth() const;

    virtual int noOfGroupedBytes() const;
    virtual int groupSpacingWidth() const;
    virtual int binaryGapWidth() const;
    /** returns true if there is a selected range in the array */
    virtual bool hasSelectedData() const;

  public: // call for action
    /** switches the array */
    virtual void resetData( char *D, int S, bool Repaint );
    /** repaint the indizes from i1 to i2 */
    virtual void repaintRange( int i1, int i2 );
    /** */
    virtual void insert( const QByteArray &D );
    /** de-/selects all data */
    virtual void selectAll( bool select );

  // clipboard interaction
    virtual void copy();
    virtual void cut();
    virtual void paste();

  // zooming
//     virtual void zoomIn( int PointInc );
//     virtual void zoomIn();
//     virtual void zoomOut( int PointInc );
//     virtual void zoomOut();
//     virtual void zoomTo( int PointSize );
//     virtual void unZoom();


  protected:
    KHE::KBytesEdit* BytesEdit;
};

#endif

