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
  Marking.unset();
  ChangedRanges.clear();
}


void KBufferRanges::setMarking( KSection M )
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


void KBufferRanges::setSelection( KSection S )
{
  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection );
  Selection = S;
  addChangedRange( Selection );
}

void KBufferRanges::setSelectionStart( int StartIndex )
{
  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection );

  Selection.setStart( StartIndex );
}


void KBufferRanges::setSelectionEnd( int EndIndex )
{
  KSection OldSelection = Selection;
  Selection.setEnd( EndIndex );

  // TODO: think about rather building a diff of the sections
  if( !OldSelection.isValid() )
  {
    addChangedRange( Selection );
    return;
  }
  if( !Selection.isValid() )
  {
    addChangedRange( OldSelection );
    return;
  }

  if( OldSelection == Selection )
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


void KBufferRanges::removeSelection( int id )
{
  if( id > 0 )
    return;

  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection );

  Selection.cancel();
  return;
}


bool KBufferRanges::overlapsSelection( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  if( Selection.overlaps(KSection(FirstIndex,LastIndex)) )
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


const KSection *KBufferRanges::firstOverlappingSelection( KSection Range ) const
{
  if( Selection.overlaps(Range) )
    return &Selection;

  return 0L;
}


const KSection *KBufferRanges::overlappingMarking( KSection Range ) const
{
  if( Marking.overlaps(Range) )
    return &Marking;

  return 0L;
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
  for( KCoordRangeList::const_iterator R=ChangedRanges.begin(); R!=ChangedRanges.end(); ++R )
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


void KBufferRanges::addChangedRange( KSection S )
{
  addChangedRange( KCoordRange(Layout->coordOfIndex(S.start()),Layout->coordOfIndex(S.end())) );
}


void KBufferRanges::addChangedRange( const KCoordRange &NewRange )
{
  ChangedRanges.addCoordRange( NewRange );

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
