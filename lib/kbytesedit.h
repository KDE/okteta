/***************************************************************************
                          kbytesedit.h  -  description
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


#ifndef KHE_UI_KBYTESEDIT_H
#define KHE_UI_KBYTESEDIT_H


// lib specific
#include "khexedit.h"

namespace KHEUI
{

class KBytesEditPrivate;

/** the beginner's hex edit widget ;)
  *
  * It hides the concept of the KAbstractByteArrayModel and accepts a pure pointer
  * to a reasonable large array of bytes
  *
  * 1. used as viewer
  *    a) static data ranges -> no changes for data pointer and length
  *       possible changes are told to the widget by repaintRange
  *    b) changing data ranges -> data pointer and length might change
  *       changes told by
  *       * resetData( char *, int size, bool repaint );
  *       *
  * 2. used as editor
  *    a) static data ranges
  *       data pointer stays the same
  *    b) changing data ranges
  *       pointer to the data changes in charge of the widget:
  *       given pointer and intermediate pointer are possibly invalid
  *       problem: who cares about the array if the data is deleted?
  *       -> solved by setAutoDelete()
  *
  *
  *@author Friedrich W. H. Kossebau
  */
//TODO: give the bytes edit widget an empty buffer as default
class KHEUI_EXPORT KBytesEdit : public KHexEdit
{
  Q_OBJECT
  //_PROPERTY( char * Data READ data )
  Q_PROPERTY( int DataSize READ dataSize )
  Q_PROPERTY( int MaxDataSize READ maxDataSize WRITE setMaxDataSize )
  Q_PROPERTY( bool AutoDelete READ isAutoDelete WRITE setAutoDelete DESIGNABLE false )


  public:
    /** creates a new widget to view/edit data
      * @param D pointer to memory
      * @param S size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is then the maximum size)
      * @param Parent parent widget
      * @param Name name for this widget
      * @param F flags
      */
    KBytesEdit( char *D, int DS, int RS, bool KM, QWidget *Parent = 0, const char *Name = 0 );
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param S size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is the maximum size)
      */
    KBytesEdit( char *D, int DS, QWidget *Parent = 0, const char *Name = 0 );
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param S size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is the maximum size)
      */
    KBytesEdit( QWidget *Parent = 0, const char *Name = 0 );
    virtual ~KBytesEdit();


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

  public: // logic value service

  public: // modification access

  public Q_SLOTS:
    /** hands over to the editor a new byte array.
      * If there exists an old one and autodelete is set the old one gets deleted.
      * @param D pointer to memory
      * @param S size of used memory
      * @param RS real size of the memory
      * @param KM keep the memory on resize (RS is the maximum size)
      */
    void setData( char *D, int S, int RS = -1, bool KM = true );

    /** sets whether the given array should be handled read only or not */
    virtual void setReadOnly( bool RO = true );
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

  Q_SIGNALS:
    /** there has been a change to the buffer */
    void contentChanged();


  protected:
    /** deletes the databuffer */
    void clean();


  protected Q_SLOTS:


  protected:
    /** */
    bool AutoDelete:1;
    /** */
    KBytesEditPrivate *d;

  private: // Disabling copy constructor and operator= - not useful
    KBytesEdit( const KBytesEdit & );
    KBytesEdit &operator=( const KBytesEdit & );
};

}

#endif
