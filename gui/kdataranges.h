/***************************************************************************
                          kdataranges.h  -  description
                             -------------------
    begin                : Sun Jun 22 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KDATARANGES_H
#define KHE_UI_KDATARANGES_H

// lib
#include "kdatalayout.h"
#include "kselection.h"
#include "kcoordrangelist.h"
// Okteta core
#include <ksectionlist.h>
// Qt
#include <QtCore/QList>

namespace KHE {
class ReplacementScope;
}


namespace KHEUI
{

/** a class to control all the ranges like marking and selections
  * holds also all modified ranges and merges them so a repaint can take its info from here
  *
  * @author Friedrich W. H.  Kossebau
  */
class KDataRanges
{
  public:
    explicit KDataRanges( KDataLayout *L );
    ~KDataRanges();

  public: // modifcation access
    void setMarking( const KHE::KSection &M );
    void setSelectionStart( int StartIndex );
    void setSelectionEnd( int StartIndex );
    void setSelection( const KHE::KSection &S );
    /** */
    void setFirstWordSelection( const KHE::KSection &S );
    /** */
    void ensureWordSelectionForward( bool Forward );

    /** removes marking and returns true if something changed */
    void removeMarking();
    /** removes selection with id and returns it */
    KHE::KSection removeSelection( int id = 0 );
    /** removes all but the standard selection and returns true if something changed */
    void removeFurtherSelections();

    void addChangedRange( const KHE::KSection &S );
    void addChangedRange( int SI, int EI );
    void addChangedRange( const KCoordRange &NewRange );
    void adaptSelectionToChange( int Pos, int RemovedLength, int InsertedLength );
    void adaptSelectionToChange( const QList<KHE::ReplacementScope> &replacementList );
    void resetChangedRanges();

    void setModified( bool M = true );
    /** removes all ranges */
    void reset();

  public: // value access
    int noOfSelections() const;
    int selectionStart() const;
    int selectionEnd() const;
    KHE::KSection selection() const;
    KHE::KSection firstWordSelection() const;
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
//    bool overlapsChanges( KHE::KSection Indizes, KHE::KSection *ChangedRange ) const;
    bool overlapsChanges( const KCoordRange &Range, KCoordRange *ChangedRange ) const;
    const KHE::KSection *firstOverlappingSelection( const KHE::KSection &Range ) const;
    const KHE::KSection *overlappingMarking( const KHE::KSection &Range ) const;


  protected:
    /** true if something changed */
    bool Modified;

    KHE::KSection Marking;
    KSelection Selection;
    /** memories first selected word on wordwise selection */
    KHE::KSection FirstWordSelection;

    KCoordRangeList ChangedRanges;

    KDataLayout *Layout;
};


inline int KDataRanges::noOfSelections()  const { return 1; }

inline int KDataRanges::selectionStart()  const { return Selection.start(); }
inline int KDataRanges::selectionEnd()    const { return Selection.end(); }
inline KHE::KSection KDataRanges::selection()  const { return Selection.section(); }
inline KHE::KSection KDataRanges::firstWordSelection()  const { return FirstWordSelection; }
inline int KDataRanges::selectionLength() const { return Selection.section().width(); }
inline bool KDataRanges::isModified()     const { return Modified; }

inline bool KDataRanges::hasSelection()             const { return Selection.isValid(); }
inline bool KDataRanges::selectionStarted()         const { return Selection.started(); }
inline bool KDataRanges::selectionJustStarted()     const { return Selection.justStarted(); }
inline bool KDataRanges::hasFirstWordSelection()    const { return FirstWordSelection.isValid(); }
inline bool KDataRanges::hasMarking()               const { return Marking.isValid(); }
inline bool KDataRanges::selectionIncludes( int Index ) const { return Selection.section().includes( Index ); }
inline bool KDataRanges::markingIncludes( int Index )   const { return Marking.includes( Index ); }

inline void KDataRanges::setModified( bool M )           { Modified = M; }

}

#endif
