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

 
#ifndef KDATABUFFER_H
#define KDATABUFFER_H


#include "ksection.h"

namespace KHE
{

/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KDataBuffer
{
  public:
    enum KWordCharType { Any, Printable, Readable };

  public:
    KDataBuffer();
    virtual ~KDataBuffer();

  public: // data access API
    /** locates working range, returns false if failed */
    virtual bool prepareRange( KSection Range ) const = 0;
    /** locates working range, returns false if failed */
    bool prepareRange( int Offset, int Length ) const;
    /** expects pointer to memory, should be in prepared range
      * it is only expected to be a valid pointer until any further call
      * to this or any modifying function
      */
    virtual const char *dataSet( KSection Section ) const = 0;
    /** expects pointer to memory, should be in prepared range
      * it is only expected to be a valid pointer until any further call
      * to this or any modifying function
      */
    const char *dataSet( int Offset, int Length ) const;
    /** call for a single byte
      + there will not be a check for the validity of the given offset
      */
    virtual char datum( int Offset ) const = 0;
    /** returns the size of the data */
    virtual int size() const = 0;

  public: // state read API
    /** is the buffer changeable? */
    virtual bool isReadOnly() const = 0;
    /** has the buffer been modified? */
    virtual bool isModified() const = 0;

  public: // modification API
    /** inserts at Position, returns length of inserted */
    virtual int insert( int Pos, const char* D, int Length );
    /** removes beginning with position as much as possible, returns length of removed
      */
    virtual int remove( KSection Section );
    /** removes beginning with position as much as possible, returns length of removed
      * convenience function, behaves as above
      */
    int remove( int Pos, int Length );
    /** replaces as much as possible, returns length of inserted */
    virtual int replace( KSection Section, const char* D, int InputLength ) = 0;
    /** replaces as much as possible, returns length of inserted */
    int replace( int Pos, int RemoveLength, const char* D, int InputLength );

    /** has the buffer been modified? */
    virtual void setModified( bool M ) = 0;


  public:
    /** copies n bytes beginning at Pos , returns number of copied bytes */
    virtual int copyTo( char* Dest, int Pos, int n ) const;

    /** returns the index of the previous */
    virtual int indexOfPreviousWordStart( int Index, KWordCharType CharType = Readable ) const;
    /** returns index of the last preceding wordchar byte,
      * if no other nonwordchar preceds that of the first byte;
      * if the actual byte is already a nonwordchar the index is returned
      */
    virtual int indexOfNextWordStart( int Index, KWordCharType CharType = Readable ) const;
    /** returns index of the last preceding wordchar byte,
      * if no other nonwordchar preceds that of the first byte;
      * if the actual byte is already a nonwordchar the index is returned
      */
    virtual int indexOfWordStart( int Index, KWordCharType CharType = Readable ) const;
    /** returns index of the last following wordchar byte or,
      * if no other nonwordchar follows, that of the last byte;
      * if the actual byte is already a nonwordchar the index is returned
      */
    virtual int indexOfWordEnd( int Index, KWordCharType CharType = Readable ) const;
    /** returns index of the first following nonwordchar byte,
      * if no other nonwordchar follows that of behind the last byte;
      * if the actual byte is already a nonwordchar the index is returned
      */
    virtual int indexOfBehindWordEnd( int Index, KWordCharType CharType = Readable ) const;

    /** returns true if the byte at position i is a char of type CharType */
    bool isWordChar( int Index, KWordCharType CharType = Readable ) const;

    /** returns the section with a word around index.
      * if there is no word the section contains only the index
      */
    virtual KSection wordSection( int Index, KWordCharType CharType = Readable  ) const;


  public: // finding API
    /** searches beginning with byte at Pos, returns -1 if nothing found */
    virtual int find( const char*, int Length, int Pos = 0 ) const = 0;
    /** searches backward beginning with byte at Pos, returns -1 if nothing found */
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

inline int KDataBuffer::replace( int Pos, int RemoveLength, const char* D, int InputLength )
{ return replace( KSection(Pos,Pos+RemoveLength-1), D, InputLength ); }

}

#endif
