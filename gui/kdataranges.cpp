/***************************************************************************
                          kdataranges.cpp  -  description
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
#include "kdataranges.h"


namespace KHEUI {

KDataRanges::KDataRanges( KDataLayout *L )
 : Modified( false ),
   Layout( L )
{
}


KDataRanges::~KDataRanges()
{
}


void KDataRanges::reset()
{
  Selection.cancel();
  FirstWordSelection.unset();
  Marking.unset();
  ChangedRanges.clear();
}


void KDataRanges::setMarking( const KHE::KSection &M )
{
  if( Marking == M )
    return;

  Marking = M;
  addChangedRange( M );
}


void KDataRanges::removeFurtherSelections()
{
  for( int i = 1; i < noOfSelections(); ++i )
    removeSelection( i );
}


void KDataRanges::setSelection( const KHE::KSection &S )
{
  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection.section() );
  Selection = S;
  addChangedRange( Selection.section() );
}

void KDataRanges::setSelectionStart( int StartIndex )
{
  bool Changed = Selection.isValid();
  if( Changed )
    addChangedRange( Selection.section() );

  Selection.setStart( StartIndex );
}


void KDataRanges::setSelectionEnd( int EndIndex )
{
  KHE::KSection OldSelection = Selection.section();
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
  KHE::KSection C( CS, CE );

  bool Changed = C.isValid();
  if( Changed )
    addChangedRange( C );
  return;
}


KHE::KSection KDataRanges::removeSelection( int id )
{
  if( id > 0 )
    return KHE::KSection();

  KHE::KSection Section = Selection.section();
  bool Changed = Section.isValid();
  if( Changed )
    addChangedRange( Section );

  Selection.cancel();
  FirstWordSelection.unset();

  return Section;
}


bool KDataRanges::overlapsSelection( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  if( Selection.section().overlaps(KHE::KSection(FirstIndex,LastIndex)) )
  {
    *SI = Selection.start();
    *EI = Selection.end();
    return true;
  }
  return false;
}


bool KDataRanges::overlapsMarking( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  if( Marking.overlaps(KHE::KSection(FirstIndex,LastIndex)) )
  {
    *SI = Marking.start();
    *EI = Marking.end();
    return true;
  }
  return false;
}


const KHE::KSection *KDataRanges::firstOverlappingSelection( const KHE::KSection &Range ) const
{
  return Selection.section().overlaps(Range) ? &Selection.section() : 0;
}


const KHE::KSection *KDataRanges::overlappingMarking( const KHE::KSection &Range ) const
{
  return Marking.overlaps(Range) ? &Marking : 0;
}

/*
bool KDataRanges::overlapsChanges( int FirstIndex, int LastIndex, int *SI, int *EI ) const
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

bool KDataRanges::overlapsChanges( KHE::KSection Indizes, KHE::KSection *ChangedRange ) const
{
  for( KHE::KSectionList::const_iterator S=ChangedRanges.begin(); S!=ChangedRanges.end(); ++S )
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
bool KDataRanges::overlapsChanges( const KCoordRange &Range, KCoordRange *ChangedRange ) const
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


void KDataRanges::addChangedRange( int SI, int EI )
{
  addChangedRange( KHE::KSection(SI,EI) );
}


void KDataRanges::addChangedRange( const KHE::KSection &S )
{
kDebug() << "adding change section "<<S.start()<<","<<S.end()<<endl;
  addChangedRange( Layout->coordRangeOfIndizes(S) );
}


void KDataRanges::addChangedRange( const KCoordRange &NewRange )
{
  ChangedRanges.addCoordRange( NewRange );
kDebug() << "as range "<<NewRange.start().pos()<<","<<NewRange.start().line()<<"-"
<<NewRange.end().pos()<<","<<NewRange.end().line()<<endl;

  Modified = true;
}


void KDataRanges::removeMarking()
{
  bool Changed = Marking.isValid();
  if( Changed )
    addChangedRange( Marking );

  Marking.unset();
}


void KDataRanges::resetChangedRanges()
{
  ChangedRanges.clear();
  Modified = false;
}


void KDataRanges::setFirstWordSelection( const KHE::KSection &Section )
{
  FirstWordSelection = Section;
  setSelection( FirstWordSelection );
}

 void KDataRanges::ensureWordSelectionForward( bool Forward )
 {
   // in the anchor not on the right side?
   if( Selection.isForward() != Forward )
   {
     setSelectionEnd( Forward ? FirstWordSelection.start() : FirstWordSelection.end()+1 );

     Selection.setForward( Forward );
   }
 }
 

void KDataRanges::adaptSelectionToChange( int Pos, int RemovedLength, int InsertedLength )
{
  Selection.adaptToChange(Pos,RemovedLength,InsertedLength );
}

}
