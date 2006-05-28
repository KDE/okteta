/***************************************************************************
                          kbufferranges.cpp  -  description
                             -------------------
    begin                : Sun Jun 22 2003
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


#include <kdebug.h>

// lib specific
#include "kbufferranges.h"


using namespace KHE;

KBufferRanges::KBufferRanges( KBufferLayout *L )
 : Modified( false ),
   Layout( L )
{
}


KBufferRanges::~KBufferRanges()
{
}


void KBufferRanges::reset()
{
  Selection.cancel();
  FirstWordSelection.unset();
  Marking.unset();
  ChangedRanges.clear();
}


void KBufferRanges::setMarking( const KSection &M )
{
  if( Marking == M )
    return;

  Marking = M;
  addChangedRange( M );
}


void KBufferRanges::removeFurtherSelections()
{
  for( int i = 1; i < noOfSelections(); ++i )
    removeSelection( i );
}


void KBufferRanges::setSelection( const KSection &S )
{
  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection.section() );
  Selection = S;
  addChangedRange( Selection.section() );
}

void KBufferRanges::setSelectionStart( int StartIndex )
{
  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection.section() );

  Selection.setStart( StartIndex );
}


void KBufferRanges::setSelectionEnd( int EndIndex )
{
  KSection OldSelection = Selection.section();
  Selection.setEnd( EndIndex );

  // TODO: think about rather building a diff of the sections
  if( !OldSelection.isValid() )
  {
    addChangedRange( Selection.section() );
    return;
  }
  if( !Selection.isValid() )
  {
    addChangedRange( OldSelection );
    return;
  }

  if( OldSelection == Selection.section() )
    return;
  int CS;
  int CE;
  // changes at the end?
  if( Selection.start() == OldSelection.start() )
  {
    CS = OldSelection.end()+1;
    CE = Selection.end();
    if( CE < CS )
    {
      CS = Selection.end()+1;
      CE = OldSelection.end();
    }
  }
  // changes at the start?
  else if( Selection.end() == OldSelection.end() )
  {
    CS = OldSelection.start();
    CE = Selection.start()-1;
    if( CE < CS )
    {
      CS = Selection.start();
      CE = OldSelection.start()-1;
    }
  }
  // change over the anchor
  else
  {
    CS = OldSelection.start();
    CE = Selection.end();
    if( CE < CS )
    {
      CS = Selection.start();
      CE = OldSelection.end();
    }
  }
  KSection C( CS, CE );

  bool Changed = C.isValid();
  if( Changed )
    addChangedRange( C );
  return;
}


KSection KBufferRanges::removeSelection( int id )
{
  if( id > 0 )
    return KSection();

  KSection Section = Selection.section();
  bool Changed = Section.isValid();
  if( Changed )
    addChangedRange( Section );

  Selection.cancel();
  FirstWordSelection.unset();

  return Section;
}


bool KBufferRanges::overlapsSelection( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  if( Selection.section().overlaps(KSection(FirstIndex,LastIndex)) )
  {
    *SI = Selection.start();
    *EI = Selection.end();
    return true;
  }
  return false;
}


bool KBufferRanges::overlapsMarking( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  if( Marking.overlaps(KSection(FirstIndex,LastIndex)) )
  {
    *SI = Marking.start();
    *EI = Marking.end();
    return true;
  }
  return false;
}


const KSection *KBufferRanges::firstOverlappingSelection( const KSection &Range ) const
{
  return Selection.section().overlaps(Range) ? &Selection.section() : 0;
}


const KSection *KBufferRanges::overlappingMarking( const KSection &Range ) const
{
  return Marking.overlaps(Range) ? &Marking : 0;
}

/*
bool KBufferRanges::overlapsChanges( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  for( KCoordRangeList::const_iterator S=ChangedRanges.begin(); S!=ChangedRanges.end(); ++S )
  {
    if( (*S).overlaps(KBuff(FirstIndex,LastIndex)) )
    {
      *SI = (*S).start();
      *EI = (*S).end();
      return true;
    }
  }

  return false;
}

bool KBufferRanges::overlapsChanges( KSection Indizes, KSection *ChangedRange ) const
{
  for( KSectionList::const_iterator S=ChangedRanges.begin(); S!=ChangedRanges.end(); ++S )
  {
    if( (*S).overlaps(Indizes) )
    {
      *ChangedRange = *S;
      return true;
    }
  }

  return false;
}
*/
bool KBufferRanges::overlapsChanges( const KCoordRange &Range, KCoordRange *ChangedRange ) const
{
  // TODO: add a lastusedrange pointer for quicker access
  for( KCoordRangeList::ConstIterator R=ChangedRanges.begin(); R!=ChangedRanges.end(); ++R )
  {
    if( (*R).overlaps(Range) )
    {
      *ChangedRange = *R;
      return true;
    }
  }

  return false;
}


void KBufferRanges::addChangedRange( int SI, int EI )
{
  addChangedRange( KSection(SI,EI) );
}


void KBufferRanges::addChangedRange( const KSection &S )
{
kDebug() << "adding change section "<<S.start()<<","<<S.end()<<endl;
  addChangedRange( Layout->coordRangeOfIndizes(S) );
}


void KBufferRanges::addChangedRange( const KCoordRange &NewRange )
{
  ChangedRanges.addCoordRange( NewRange );
kDebug() << "as range "<<NewRange.start().pos()<<","<<NewRange.start().line()<<"-"
<<NewRange.end().pos()<<","<<NewRange.end().line()<<endl;

  Modified = true;
}


void KBufferRanges::removeMarking()
{
  bool Changed = Marking.isValid();
  if( Changed )
    addChangedRange( Marking );

  Marking.unset();
}


void KBufferRanges::resetChangedRanges()
{
  ChangedRanges.clear();
  Modified = false;
}


void KBufferRanges::setFirstWordSelection( const KSection &Section )
{
  FirstWordSelection = Section;
  setSelection( FirstWordSelection );
}

 void KBufferRanges::ensureWordSelectionForward( bool Forward )
 {
   // in the anchor not on the right side?
   if( Selection.isForward() != Forward )
   {
     setSelectionEnd( Forward ? FirstWordSelection.start() : FirstWordSelection.end()+1 );

     Selection.setForward( Forward );
   }
 }
 

void KBufferRanges::adaptSelectionToChange( int Pos, int RemovedLength, int InsertedLength )
{
  Selection.adaptToChange(Pos,RemovedLength,InsertedLength );
}
