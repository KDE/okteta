/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_ABSTRACTBYTEARRAYMODEL_H
#define OKTETA_ABSTRACTBYTEARRAYMODEL_H

// lib
#include "oktetacore_export.h"
// commonlib
#include "section.h"
// Qt
#include <QtCore/QObject>
#include <QtCore/QByteArray>


namespace KDE {
class ArrayChangeMetricsList;
}

namespace Okteta
{

/** could it be useful to hide the data access behind an iterator? *
class KDataBufferIterator
{
  public:
    bool hasNext();
    char next();
  protected:

  protected:
    char *
    int Length;
}

bool KDataBufferIterator::hasNext()
{
}
// this function should be simple as possible
char KDataBufferIterator::next()
{
  // if next span is empty
  if(  )
    return *NextChar++;
}
*/
/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *
  * Operations on the data:
  *
  * Finding: is implemented stateless. FindNext has to be done by perhaps a FindManager
  * Replacing: not available. Implement within a ReplaceManager
  *
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT AbstractByteArrayModel : public QObject
{
  friend class KAbstractByteArrayModelIterator;

  Q_OBJECT

  protected:
    AbstractByteArrayModel();
  public:
    virtual ~AbstractByteArrayModel();


  public: // data access API
    /** locates working range
      * The idea behind is to tell buffer which range will be requested in the following time,
      * so that e.g. the corresponding pages will be loaded in advance
      * TODO: do we really need this? Where will this lead to real enhancements?
      * @param Range
      * @return @c true if successfully, @c false otherwise
      */
    //virtual bool prepareRange( const Section &Range ) const = 0;
    /** convenience function, same as above */
    //bool prepareRange( int Offset, int Length ) const;

    /** creates an iterator to */
    //virtual KDataBufferIterator *iterator() const = 0;
    /** expects pointer to memory, should be in prepared range
      * it is only expected to be a valid pointer until any further call
      * to this or any modifying function
      * @param Section 
      * @return pointer to 
      */
    //virtual const char *dataSet( const Section &Section ) const = 0;
    /** convenience function, same as above */
    //const char *dataSet( int Offset, int Length ) const;

    /** requests a single byte
      * if the offset is not valid the behaviout is undefined
      * @param offset offset of the datum requested
      */
    virtual char datum( unsigned int offset ) const = 0;

    /**
      * @return the size of the data
      */
    virtual int size() const = 0;


  public: // state read API
    /**
      * Default returns @c true.
      * @return @c true if the buffer can only be read, @c false if writing is allowed
      */
    virtual bool isReadOnly() const;
    /**
      * @return @c true if the buffer has been modified, @c false otherwise
      */
    virtual bool isModified() const = 0;


  public: // modification API
    /** inserts bytes copied from the given source at Position.
      * The original data beginnung at the position is moved
      * with the buffer enlarged as needed
      * If the buffer is readOnly this is a noop and returns 0.
      * @param offset
      * @param insertData data source
      * @param insertLength number of bytes to copy
      * @return length of inserted data
      */
    virtual int insert( int offset, const char *insertData, int insertLength );
    int insert( int offset, const QByteArray &insertData );

    /** removes beginning with position as much as possible
      * @param removeSection
      * @return length of removed data
      */
    virtual int remove( const KDE::Section& removeSection );
    /** convenience function, behaves as above */
    int remove( int offset, int removeLength );

    /** replaces as much as possible
      * @param removeSection
      * @param insertData
      * @param insertLength
      * @return length of inserted data
      */
    virtual unsigned int replace( const KDE::Section& removeSection, const char* insertData, unsigned int insertLength ) = 0;
    /** convenience function, behaves as above */
    unsigned int replace( const KDE::Section& removeSection, const QByteArray& insertData );
    /** convenience function, behaves as above */
    unsigned int replace( unsigned int offset, unsigned int removeLength,
                          const char *insertData, unsigned int insertLength );

    // todo use parameters grouped differrently?
    /** moves the second section before the start of the first
      * which is the same as moving the first behind the second.
      * @param firstStart position of the data where the section should be moved behind
      * @param secondSection data section to be moved
      * @return @true if operation was successful, @false otherwise
      */
    virtual bool swap( int firstStart, const KDE::Section& secondSection ) = 0;

    /**
     * fills the buffer with the FillChar. If the buffer is to small it will be extended as much as possible.
     * @param fillChar char to use
     * @param offset position where the filling starts
     * @param fillLength number of chars to fill. If Length is -1, the buffer is filled till the end.
     * @return number of filled characters
     */
    virtual int fill( const char fillChar, unsigned int offset = 0, int fillLength = -1 ) = 0;
    int fill( const char fillChar, const KDE::Section& fillSection );

    /** sets a single byte
     * if the offset is not valid the behaviour is undefined
     * @param offset offset of the datum requested
     * @param value new byte value
     */
    virtual void setDatum( unsigned int offset, const char value ) = 0;

    /** sets the modified flag for the buffer
      * @param modified
      */
    virtual void setModified( bool modified ) = 0;

    /** sets the readonly flag for the byte array if this is possible.
      * Default implementation does not do anything.
      * @param isReadOnly new state
      */
    virtual void setReadOnly( bool isReadOnly );


  public: // service functions
    /** copies the data of the section into a given array Dest. If the section extends the buffers range
      * the section is limited to the buffer's end. If the section is invalid the behaviour is undefined.
      * @param dest pointer to a char array large enough to hold the copied section
      * @param copySection
      * @return number of copied bytes
      */
    virtual int copyTo( char *dest, const KDE::Section& copySection ) const;
    /** convenience function, behaves as above */
    int copyTo( char *dest, int offset, unsigned int copyLength ) const;


  public: // finding API
    /** searches beginning with byte at Pos.
      * @param pattern
      * @param patternLength length of search string
      * @param fromOffset the position to start the search
      * @return index of the first  or -1
      */
    virtual int indexOf( const char *pattern, int patternLength, int fromOffset = 0 ) const;
    int indexOf( const QByteArray &pattern, int fromOffset = 0 ) const;
    /** searches for a given data string
      * The section limits the data within which the key has to be found
      * If the end of the section is lower than the start the search continues at the start???
      * @param 
      * @param Length length of search string
      * @param Section section within the keydata is to be found
      * @return index of the first occurrence or -1
      */
//     virtual int indexOf( const char*KeyData, int Length, const Section &Section ) const { return -1; }//= 0;
    /** searches backward beginning with byte at Pos.
      * @param 
      * @param patternLength length of search string
      * @param fromOffset the position to start the search. If -1 the search starts at the end.
      * @return index of the first  or -1
      */
    virtual int lastIndexOf( const char *pattern, int patternLength, int fromOffset = -1 ) const;
    int lastIndexOf( const QByteArray &pattern, int fromOffset = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  Q_SIGNALS:
    // TODO: how to deal replacing with fixed size of buffer?
    void contentsChanged( const KDE::ArrayChangeMetricsList &changeList );

    void readOnlyChanged( bool isReadOnly );
    void modifiedChanged( bool isModified );

    void searchedBytes( int bytes ) const;
};

inline int AbstractByteArrayModel::insert( int Pos, const QByteArray &Source )
{ return insert( Pos, Source.data(), Source.size() ); }

inline int AbstractByteArrayModel::remove( int Pos, int Length )
{ return remove( KDE::Section(Pos,Pos+Length-1) ); }

inline unsigned int AbstractByteArrayModel::replace( const KDE::Section& destSection, const QByteArray& source )
{ return replace( destSection, source.data(), source.size() );}

inline unsigned int AbstractByteArrayModel::replace( unsigned int Pos, unsigned int RemoveLength,
                                 const char* D, unsigned int InputLength )
{ return replace( KDE::Section(Pos,Pos+RemoveLength-1), D, InputLength ); }

inline int AbstractByteArrayModel::fill( const char FillChar, const KDE::Section& section )
{ return fill( FillChar, section.start(), section.width() ); }

inline int AbstractByteArrayModel::copyTo( char *dest, int offset, unsigned int copyLength ) const
{ return copyTo( dest, KDE::Section::fromWidth(offset,copyLength) ); }

inline int AbstractByteArrayModel::indexOf( const QByteArray& Data, int From ) const
{ return indexOf( Data.constData(), Data.size(), From ); }

inline int AbstractByteArrayModel::lastIndexOf( const QByteArray& Data, int From ) const
{ return lastIndexOf( Data.constData(), Data.size(), From ); }

}

#endif
