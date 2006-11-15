/***************************************************************************
                          kbytearrayeditprivate.h  -  description
                             -------------------
    begin                : Die Jul 9 2003
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


#ifndef KBYTESEDITPRIVATE_H
#define KBYTESEDITPRIVATE_H


// corelib specific
#include <kbytearraymodel.h>
// uilib specific
#include <khexedit.h>

class KByteArrayEdit;


class KByteArrayEditPrivate
{
    friend class KByteArrayEdit;

  public:
    KByteArrayEditPrivate( KByteArrayEdit *Parent );
    ~KByteArrayEditPrivate();


  public: // value access
    /** returns the pointer to the actual byte array. This can be but must not be the one
      * that was given in the constructor.
      */
    char *data() const;
    /** returns the size of the actual byte array */
    int dataSize() const;
    /** returns the maximal allowed size for the byte array */
    int maxDataSize () const;
    /** returns whether autodelete is set for the byte array */
    bool isAutoDelete() const;
    /** returns whether the memory of the byte array is kept */
    bool keepsMemory() const;


  public:
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param S size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is the maximum size)
      */
    void setData( char *D, int S, int RS = -1, bool KM = true );

    /** sets whether the given array should be handled read only or not */
    void setReadOnly( bool RO = true );
    /** sets the maximal size of the actual byte array. If the actual array is already larger
      * it will not be modified but there can be only done non-inserting actions
      * until the array's is below the limit
      */
    void setMaxDataSize( int MS );
    /** sets whether the array should be deleted on the widget's end or if a new array is set.
      * Default is false
      */
    void setAutoDelete( bool AD = true );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool KM = true );

    /** repaint the indizes from i1 to i2 */
    void repaintRange( int i1, int i2 );


  protected:
    /** */
    KHECore::KByteArrayModel *Model;
    KHEUI::KHexEdit *HexEdit;
};

#endif
