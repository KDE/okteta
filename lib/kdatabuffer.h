/***************************************************************************
                          kdatabuffer.h  -  description
                             -------------------
    begin                : Mit Mai 14 2003
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


#ifndef KHE_KDATABUFFER_H
#define KHE_KDATABUFFER_H


// lib specific
#include "ksection.h"
#include "khexedit_export.h"

namespace KHE
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

class KHEXEDIT_EXPORT KDataBuffer
{
  friend class KDataBufferIterator;


  protected:
    KDataBuffer();
  public:
    virtual ~KDataBuffer();


  public: // data access API
    /** locates working range
      * The idea behind is to tell buffer which range will be requested in the following time,
      * so that e.g. the corresponding pages will be loaded in advance
      * TODO: do we really need this? Where will this lead to real enhancements?
      * @param Range
      * @return @c true if successfull, @c false otherwise
      */
    virtual bool prepareRange( KSection Range ) const = 0;
    /** convenience function, same as above */
    bool prepareRange( int Offset, int Length ) const;

    /** creates an iterator to */
    //virtual KDataBufferIterator *iterator() const = 0;
    /** expects pointer to memory, should be in prepared range
      * it is only expected to be a valid pointer until any further call
      * to this or any modifying function
      * @param Section 
      * @return pointer to 
      */
    virtual const char *dataSet( KSection Section ) const = 0;
    /** convenience function, same as above */
    const char *dataSet( int Offset, int Length ) const;

    /** requests a single byte
      * if the offset is not valid the behaviout is undefined
      * @param Offset offset of the datum requested
      */
    virtual char datum( unsigned int Offset ) const = 0;

    /**
      * @return the size of the data
      */
    virtual int size() const = 0;


  public: // state read API
    /**
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
      * @param Pos
      * @param Source data source
      * @param SourceLength number of bytes to copy
      * @return length of inserted data
      */
    virtual int insert( int Pos, const char* Source, int SourceLength );

    /** removes beginning with position as much as possible
      * @param Section
      * @return length of removed data
      */
    virtual int remove( KSection Section );
    /** convenience function, behaves as above */
    int remove( int Pos, int Length );

    /** replaces as much as possible
      * @param DestSection
      * @param Source
      * @param SourceLength
      * @return length of replacced data
      */
    virtual unsigned int replace( KSection DestSection, const char* Source, unsigned int SourceLength ) = 0;
    /** convenience function, behaves as above */
    unsigned int replace( unsigned int Pos, unsigned int RemoveLength,
                          const char* Source, unsigned int SourceLength );

    /** moves a part of the data to a new position, while floating the other data around
      * when moving to a higher place the length of the block must be taken into account
      * if the new positions extend beyond the buffers end the section is moved to the end.
      * @param DesPos position of the data where the section should be moved behind
      * @param SourceSection data section to be moved
      * @return new pos of moved data or old, if failed
      */
    virtual int move( int DestPos, KSection SourceSection ) = 0;

    /**
     * fills the buffer with the FillChar. If the buffer is to small it will be extended as much as possible.
     * @param FillChar char to use
     * @param Length number of chars to fill. If Length is -1, the current buffer length is used.
     * @param Pos position where the filling starts
     * @return number of filled characters
     */
    virtual int fill( const char FillChar, int Length = -1, unsigned int Pos = 0 ) = 0;

    /** sets a single byte
     * if the offset is not valid the behaviour is undefined
     * @param Offset offset of the datum requested
     * @param Char new byte value
     */
    virtual void setDatum( unsigned int Offset, const char Char ) = 0;

    /** sets the modified flag for the buffer
      * @param M
      */
    virtual void setModified( bool M ) = 0;


  public: // service functions
    /** copies the data of the section into a given array Dest. If the section extends the buffers range
      * the section is limited to the buffer's end. If the section is invalid the behaviour is undefined.
      * @param Dest pointer to a char array large enough to hold the copied section
      * @param Source
      * @return number of copied bytes
      */
    virtual int copyTo( char* Dest, KSection Source ) const;
    /** convenience function, behaves as above */
    int copyTo( char* Dest, int Pos, int n ) const;


  public: // finding API
    /** searches beginning with byte at Pos.
      * @param 
      * @param Length length of search string
      * @param Pos the position to start the search
      * @return index of the first  or -1
      */
    //virtual int find( const char*, int Length, int Pos = 0 ) const = 0;
    /** searches for a given data string
      * The section limits the data within which the key has to be found
      * If the end of the section is lower then the start the search continues at the start???
      * @param 
      * @param Length length of search string
      * @param Section section within the keydata is to be found
      * @return index of the first occurence or -1
      */
    virtual int find( const char*KeyData, int Length, KSection Section ) const = 0;
    /** searches backward beginning with byte at Pos.
      * @param 
      * @param Length length of search string
      * @param Pos the position to start the search. If -1 the search starts at the end.
      * @return index of the first  or -1
      */
    virtual int rfind( const char*, int Length, int Pos = -1 ) const = 0;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */
};


inline KDataBuffer::KDataBuffer() {}
inline KDataBuffer::~KDataBuffer() {}

inline bool KDataBuffer::prepareRange( int Offset, int Length ) const
{ return prepareRange( KSection(Offset,Offset+Length-1) ); }

inline const char *KDataBuffer::dataSet( int Offset, int Length ) const
{ return dataSet( KSection(Offset,Offset+Length-1) ); }

inline int KDataBuffer::remove( int Pos, int Length )
{ return remove( KSection(Pos,Pos+Length-1) ); }

inline unsigned int KDataBuffer::replace( unsigned int Pos, unsigned int RemoveLength,
                                 const char* D, unsigned int InputLength )
{ return replace( KSection(Pos,Pos+RemoveLength-1), D, InputLength ); }

inline bool KDataBuffer::isReadOnly() const { return false; }

}

#endif
