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

// qt specific
#include <qwidget.h>
// kde specific
#include <khexedit/byteseditinterface.h>
#include <khexedit/valuecolumninterface.h>
#include <khexedit/charcolumninterface.h>
#include <khexedit/zoominterface.h>
#include <khexedit/clipboardinterface.h>


namespace KHE {
class KBytesEdit;
}

/**
   hex editor/viewer.

   @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
   @version 0.1
 **/
class KBytesEditWidget : public QWidget, public KHE::BytesEditInterface,
                         public KHE::ValueColumnInterface, public KHE::CharColumnInterface,
                         public KHE::ZoomInterface, public KHE::ClipboardInterface
{
  Q_OBJECT

  public:
    /** constructor API as demanded by KGenericFactory */
    KBytesEditWidget( QWidget *parent, const char *name, const QStringList & = QStringList() );

  public: // bytesedit interface
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param DS size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is the maximum size)
      */
    virtual void setData( char *D, int DS, int RS = -1, bool KM = true );

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
    /** repaint the indizes from i1 to i2 */
    virtual void repaintRange( int i1, int i2 );

  public: // cursor interface
    /** */
    virtual void setCursorPosition( int Index );
//     virtual bool tabChangesFocus() const;

  public: // layout interface ??
    /** sets the resizestyle for the value column. Default is KHE::FullSizeUsage */
    virtual void setResizeStyle( KResizeStyle Style );
    /** sets the number of bytes per line, switching the resize style to KHE::NoResize */
    virtual void setNoOfBytesPerLine( int NoCpL );

    virtual int noOfBytesPerLine() const;
    virtual KResizeStyle resizeStyle() const;

  public: // value column
    /** sets the format of the value column. Default is KHE::HexadecimalCoding */
    virtual void setCoding( KCoding C );
    /** sets the spacing between the bytes in pixels */
    virtual void setByteSpacingWidth( int BSW );
    /** sets the numbers of grouped bytes, 0 means no grouping, Default is 4 */
    virtual void setNoOfGroupedBytes( int NoGB );
    /** sets the spacing between the groups in pixels */
    virtual void setGroupSpacingWidth( int GSW );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BGW spacing in the middle of a binary in pixels
      */
    virtual void setBinaryGapWidth( int BGW );

    virtual KCoding coding() const;
    virtual int byteSpacingWidth() const;

    virtual int noOfGroupedBytes() const;
    virtual int groupSpacingWidth() const;
    virtual int binaryGapWidth() const;

  public:  // char column
    /** sets whether "unprintable" chars (>32) should be displayed in the char column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    virtual void setShowUnprintable( bool SU = true );
    /** sets the substitute character for "unprintable" chars
      * returns true if there was a change
      */
    virtual void setSubstituteChar( QChar SC );
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed. */
    virtual void setEncoding( KEncoding C );
    /** returns true if "unprintable" chars (>32) are displayed in the char column
      * with their corresponding character, default is false
      */
    virtual bool showUnprintable() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    virtual QChar substituteChar() const;
    /** */
    virtual KEncoding encoding()   const;

  public: // edit interface
    /** */
    virtual void insert( const QByteArray &D );
    /** de-/selects all data */
    virtual void selectAll( bool select );
    /** returns true if there is a selected range in the array */
    virtual bool hasSelectedData() const;

  public slots:
  // bytesedit interface
    virtual void setReadOnly( bool RO = true );
    virtual void setOverwriteOnly( bool b );
    virtual void setOverwriteMode( bool b );
    virtual void setModified( bool b );

  // clipboard interface
    virtual void copy();
    virtual void cut();
    virtual void paste();

  // zooming interface
    virtual void zoomIn( int PointInc );
    virtual void zoomIn();
    virtual void zoomOut( int PointDec );
    virtual void zoomOut();
    virtual void zoomTo( int PointSize );
    virtual void unZoom();

  signals:
  // clipboard interface
    virtual void copyAvailable( bool Really );

  protected:
    KHE::KBytesEdit* BytesEdit;
};

#endif
