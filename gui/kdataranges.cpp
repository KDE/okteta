/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

#include "kdataranges.h"

// lib
#include "bytearraytablelayout.h"
// Okteta core
#include <arraychangemetricslist.h>


namespace KHEUI {

KDataRanges::KDataRanges( ByteArrayTableLayout *L )
 : Modified( false ),
   Layout( L )
{
}


KDataRanges::~KDataRanges()
{
}


void KDataRanges::reset()
{
  mSelection.cancel();
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
  bool Changed = mSelection.isValid();
  if( Changed )
    addChangedRange( mSelection.section() );
  mSelection = S;
  addChangedRange( mSelection.section() );
}

void KDataRanges::setSelectionStart( int StartIndex )
{
  bool Changed = mSelection.isValid();
  if( Changed )
    addChangedRange( mSelection.section() );

  mSelection.setStart( StartIndex );
}


void KDataRanges::setSelectionEnd( int EndIndex )
{
  KHE::KSection OldSelection = mSelection.section();
  mSelection.setEnd( EndIndex );

  // TODO: think about rather building a diff of the sections
  if( !OldSelection.isValid() )
  {
    addChangedRange( mSelection.section() );
    return;
  }
  if( !mSelection.isValid() )
  {
    addChangedRange( OldSelection );
    return;
  }

  if( OldSelection == mSelection.section() )
    return;
  int CS;
  int CE;
  // changes at the end?
  if( mSelection.start() == OldSelection.start() )
  {
    CS = OldSelection.nextBehindEnd();
    CE = mSelection.end();
    if( CE < CS )
    {
      CS = mSelection.nextBehindEnd();
      CE = OldSelection.end();
    }
  }
  // changes at the start?
  else if( mSelection.end() == OldSelection.end() )
  {
    CS = OldSelection.start();
    CE = mSelection.nextBeforeStart();
    if( CE < CS )
    {
      CS = mSelection.start();
      CE = OldSelection.nextBeforeStart();
    }
  }
  // change over the anchor
  else
  {
    CS = OldSelection.start();
    CE = mSelection.end();
    if( CE < CS )
    {
      CS = mSelection.start();
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

  KHE::KSection Section = mSelection.section();
  bool Changed = Section.isValid();
  if( Changed )
    addChangedRange( Section );

  mSelection.cancel();
  FirstWordSelection.unset();

  return Section;
}


bool KDataRanges::overlapsSelection( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  if( mSelection.section().overlaps(KHE::KSection(FirstIndex,LastIndex)) )
  {
    *SI = mSelection.start();
    *EI = mSelection.end();
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
  return mSelection.section().overlaps(Range) ? &mSelection.section() : 0;
}


const KHE::KSection *KDataRanges::overlappingMarking( const KHE::KSection &Range ) const
{
  return Marking.overlaps(Range) ? &Marking : 0;
}

/*
bool KDataRanges::overlapsChanges( int FirstIndex, int LastIndex, int *SI, int *EI ) const
{
  for( CoordRangeList::const_iterator S=ChangedRanges.begin(); S!=ChangedRanges.end(); ++S )
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
bool KDataRanges::overlapsChanges( const CoordRange &Range, CoordRange *ChangedRange ) const
{
  // TODO: add a lastusedrange pointer for quicker access
  for( CoordRangeList::ConstIterator R=ChangedRanges.begin(); R!=ChangedRanges.end(); ++R )
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
// kDebug() << "adding change section "<<S.start()<<","<<S.end();
  addChangedRange( Layout->coordRangeOfIndizes(S) );
}


void KDataRanges::addChangedRange( const CoordRange &NewRange )
{
  ChangedRanges.addCoordRange( NewRange );
// kDebug() << "as range "<<NewRange.start().pos()<<","<<NewRange.start().line()<<"-"
// <<NewRange.end().pos()<<","<<NewRange.end().line()<<endl;

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
   if( mSelection.isForward() != Forward )
   {
     setSelectionEnd( Forward ? FirstWordSelection.start() : FirstWordSelection.nextBehindEnd() );

     mSelection.setForward( Forward );
   }
 }


void KDataRanges::adaptToChanges( const KHE::ArrayChangeMetricsList& changeList )
{
    foreach( const KHE::ArrayChangeMetrics& change, changeList )
    {
        //TODO: change parameters to ArrayChangeMetrics
        switch( change.type() )
        {
        case KHE::ArrayChangeMetrics::Replacement:
        {
            const int diff = change.lengthChange();
            const int last = (diff == 0) ? change.insertLength() : Layout->length() + qAbs(diff);
            addChangedRange( change.offset(), last );

            if( mSelection.isValid() )
                mSelection.adaptToReplacement( change.offset(), change.removeLength(), change.insertLength() );
            break;
        }
        case KHE::ArrayChangeMetrics::Swapping:
            addChangedRange( change.offset(), change.secondEnd() );

            if( mSelection.isValid() )
                mSelection.adaptToSwap( change.offset(), change.secondStart(), change.secondLength() );
        default:
            ;
        }
    }
}

}
