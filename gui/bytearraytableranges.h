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

#ifndef OKTETA_BYTEARRAYTABLERANGES_H
#define OKTETA_BYTEARRAYTABLERANGES_H

// lib
#include "selection.h"
#include "coordrangelist.h"
#include "oktetagui_export.h"
// Okteta core
#include <sectionlist.h>

namespace KDE {
class ArrayChangeMetricsList;
}


namespace Okteta
{

class ByteArrayTableLayout;

/** a class to control all the ranges like marking and selections
  * holds also all modified ranges and merges them so a repaint can take its info from here
  *
  * @author Friedrich W. H.  Kossebau
  */
// TODO: split info about ranges from info about dirty ranges into a second class
class OKTETAGUI_EXPORT ByteArrayTableRanges
{
  public:
    explicit ByteArrayTableRanges( ByteArrayTableLayout* layout );
    ~ByteArrayTableRanges();

  public: // modifcation access
    void setMarking( const KDE::Section &M );
    void setSelectionStart( int StartIndex );
    void setSelectionEnd( int StartIndex );
    void setSelection( const KDE::Section &S );
    /** */
    void setFirstWordSelection( const KDE::Section &S );
    /** */
    void ensureWordSelectionForward( bool Forward );

    /** removes marking and returns true if something changed */
    void removeMarking();
    /** removes selection with id and returns it */
    KDE::Section removeSelection( int id = 0 );
    /** removes all but the standard selection and returns true if something changed */
    void removeFurtherSelections();

    /** assumes all added lines to overlap */
    void addChangedOffsetLines( const KDE::Section& changesLines );

    void addChangedRange( const KDE::Section &S );
    void addChangedRange( int SI, int EI );
    void addChangedRange( const CoordRange &NewRange );
    void adaptToChanges( const KDE::ArrayChangeMetricsList& changeList, int oldLength );
    void resetChangedRanges();

    void setModified( bool M = true );
    /** removes all ranges */
    void reset();

  public: // value access
    int noOfSelections() const;
    int selectionStart() const;
    int selectionEnd() const;
    KDE::Section selection() const;
    KDE::Section firstWordSelection() const;
    int selectionLength() const;
    bool isModified() const;
    KDE::Section changedOffsetLines() const;

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
//    bool overlapsChanges( KDE::Section Indizes, KDE::Section *ChangedRange ) const;
    bool overlapsChanges( const CoordRange &Range, CoordRange *ChangedRange ) const;
    const KDE::Section *firstOverlappingSelection( const KDE::Section &Range ) const;
    const KDE::Section *overlappingMarking( const KDE::Section &Range ) const;


  protected:
    /** true if something changed */
    bool Modified;

    KDE::Section Marking;
    Selection mSelection;
    /** memories first selected word on wordwise selection */
    KDE::Section FirstWordSelection;

    /** lines that were added or removed */
    KDE::Section mChangedOffsetLines;

    CoordRangeList ChangedRanges;

    ByteArrayTableLayout *Layout;
};


inline int ByteArrayTableRanges::noOfSelections()  const { return 1; }

inline int ByteArrayTableRanges::selectionStart()  const { return mSelection.start(); }
inline int ByteArrayTableRanges::selectionEnd()    const { return mSelection.end(); }
inline KDE::Section ByteArrayTableRanges::selection()  const { return mSelection.section(); }
inline KDE::Section ByteArrayTableRanges::firstWordSelection()  const { return FirstWordSelection; }
inline int ByteArrayTableRanges::selectionLength() const { return mSelection.section().width(); }
inline bool ByteArrayTableRanges::isModified()     const { return Modified; }
inline KDE::Section ByteArrayTableRanges::changedOffsetLines() const { return mChangedOffsetLines; }

inline bool ByteArrayTableRanges::hasSelection()             const { return mSelection.isValid(); }
inline bool ByteArrayTableRanges::selectionStarted()         const { return mSelection.started(); }
inline bool ByteArrayTableRanges::selectionJustStarted()     const { return mSelection.justStarted(); }
inline bool ByteArrayTableRanges::hasFirstWordSelection()    const { return FirstWordSelection.isValid(); }
inline bool ByteArrayTableRanges::hasMarking()               const { return Marking.isValid(); }
inline bool ByteArrayTableRanges::selectionIncludes( int Index ) const { return mSelection.section().includes( Index ); }
inline bool ByteArrayTableRanges::markingIncludes( int Index )   const { return Marking.includes( Index ); }

inline void ByteArrayTableRanges::setModified( bool M )           { Modified = M; }

}

#endif
