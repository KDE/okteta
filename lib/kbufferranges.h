/***************************************************************************
                          kbufferranges.h  -  description
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


#ifndef KHE_KBUFFERRANGES_H
#define KHE_KBUFFERRANGES_H

// lib specific
#include "kbufferlayout.h"
#include "kselection.h"
#include "ksectionlist.h"
#include "kcoordrangelist.h"

namespace KHE
{

/** a class to control all the ranges like marking and selections
  * holds also all modified ranges and merges them so a repaint can take its info from here
  *
  * @author Friedrich W. H.  Kossebau
  */
class KBufferRanges
{
  public:
    KBufferRanges( KBufferLayout *L );
    ~KBufferRanges();

  public: // modifcation access
    void setMarking( KSection M );
    void setSelectionStart( int StartIndex );
    void setSelectionEnd( int StartIndex );
    void setSelection( KSection S );
    /** */
    void setFirstWordSelection( KSection S );
    /** */
    void ensureWordSelectionForward( bool Forward );

    /** removes marking and returns true if something changed */
    void removeMarking();
    /** removes selection with id and returns true if something changed */
    void removeSelection( int id = 0 );
    /** removes all but the standard selection and returns true if something changed */
    void removeFurtherSelections();

    void addChangedRange( KSection S );
    void addChangedRange( int SI, int EI );
    void addChangedRange( const KCoordRange &NewRange );
    void resetChangedRanges();

    void setModified( bool M = true );
    /** removes all ranges */
    void reset();

  public: // value access
    int noOfSelections() const;
    int selectionStart() const;
    int selectionEnd() const;
    KSection selection() const;
    KSection firstWordSelection() const;
    int selectionLength() const;
    bool isModified() const;

  public: // calculated logic access
    bool hasSelection() const;
    bool hasMarking() const;
    bool selectionStarted() const;
    bool selectionJustStarted() const;
    bool hasFirstWordSelection() const;
    bool selectionIncludes( int Index ) const;
    bool markingIncludes( int Index ) const;
    // TODO: next three are deprecated
    bool overlapsSelection( int FirstIndex, int LastIndex, int *SI, int *EI ) const;
    bool overlapsMarking( int FirstIndex, int LastIndex, int *SI, int *EI ) const;
//    bool overlapsChanges( int FirstIndex, int LastIndex, int *SI, int *EI ) const;
//    bool overlapsChanges( KSection Indizes, KSection *ChangedRange ) const;
    bool overlapsChanges( const KCoordRange &Range, KCoordRange *ChangedRange ) const;
    const KSection *firstOverlappingSelection( KSection Range ) const;
    const KSection *overlappingMarking( KSection Range ) const;


  protected:
    /** true if something changed */
    bool Modified;

    KSection Marking;
    KSelection Selection;
    /** memories first selected word on wordwise selection */
    KSection FirstWordSelection;

    KCoordRangeList ChangedRanges;

    KBufferLayout *Layout;
};


inline int KBufferRanges::noOfSelections()  const { return 1; }

inline int KBufferRanges::selectionStart()  const { return Selection.start(); }
inline int KBufferRanges::selectionEnd()    const { return Selection.end(); }
inline KSection KBufferRanges::selection()  const { return Selection; }
inline KSection KBufferRanges::firstWordSelection()  const { return FirstWordSelection; }
inline int KBufferRanges::selectionLength() const { return Selection.width(); }
inline bool KBufferRanges::isModified()     const { return Modified; }

inline bool KBufferRanges::hasSelection()             const { return Selection.isValid(); }
inline bool KBufferRanges::selectionStarted()         const { return Selection.started(); }
inline bool KBufferRanges::selectionJustStarted()     const { return Selection.justStarted(); }
inline bool KBufferRanges::hasFirstWordSelection()    const { return FirstWordSelection.isValid(); }
inline bool KBufferRanges::hasMarking()               const { return Marking.isValid(); }
inline bool KBufferRanges::selectionIncludes( int Index ) const { return Selection.includes( Index ); }
inline bool KBufferRanges::markingIncludes( int Index )   const { return Marking.includes( Index ); }

inline void KBufferRanges::setModified( bool M )           { Modified = M; }

}

#endif
