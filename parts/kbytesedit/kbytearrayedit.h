/*
    This file is part of the Okteta KByteArrayEdit module, made within the KDE community.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KBYTEARRAYEDIT_H
#define KBYTEARRAYEDIT_H

// KDE
#include <khexedit/byteseditinterface.h>
#include <khexedit/valuecolumninterface.h>
#include <khexedit/charcolumninterface.h>
#include <khexedit/zoominterface.h>
#include <khexedit/clipboardinterface.h>
// Qt
#include <QtGui/QWidget>

namespace Okteta {
class ByteArrayModel;
}
namespace Okteta {
class ByteArrayColumnView;
}


/**
   hex editor/viewer.

   @author Friedrich W. H. Kossebau <kossebau@kde.org>
   @version 0.1
 **/
class KByteArrayEdit : public QWidget,
                       public KHE::BytesEditInterface,
                       public KHE::ValueColumnInterface,
                       public KHE::CharColumnInterface,
                       public KHE::ZoomInterface,
                       public KHE::ClipboardInterface
{
  Q_OBJECT
  Q_INTERFACES(
    KHE::BytesEditInterface
    KHE::ValueColumnInterface
    KHE::CharColumnInterface
    KHE::ZoomInterface
    KHE::ClipboardInterface
  )
  //_PROPERTY( char * Data READ data )
  Q_PROPERTY( int DataSize READ dataSize )
  Q_PROPERTY( int MaxDataSize READ maxDataSize WRITE setMaxDataSize )
  Q_PROPERTY( bool AutoDelete READ isAutoDelete WRITE setAutoDelete DESIGNABLE false )

  public:
    /** constructor API as demanded by KPluginFactory */
    explicit KByteArrayEdit( QWidget* parent, const QList<QVariant>& = QList<QVariant>() );
    virtual ~KByteArrayEdit();

  public: // bytesedit interface
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param DSize size of used memory
      * @param RSize real size of the memory
      * @param KM keep the memory on resize (RSize is the maximum size)
      */
    virtual void setData( char *D, int DSize, int RSize = -1, bool KM = true );

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

  public Q_SLOTS:
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

  Q_SIGNALS:
  // clipboard interface
    void copyAvailable( bool Really );

  private:
    Okteta::ByteArrayModel *Model;
    Okteta::ByteArrayColumnView *View;
};

#endif
