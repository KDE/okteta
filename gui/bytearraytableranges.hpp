/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYTABLERANGES_HPP
#define OKTETA_BYTEARRAYTABLERANGES_HPP

// lib
#include "oktetagui_export.hpp"
#include "selection.hpp"
#include "coordrangelist.hpp"
// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QScopedPointer>

namespace Okteta {
class ArrayChangeMetricsList;
class ByteArrayTableLayout;
class ByteArrayTableRangesPrivate;

/** a class to control all the ranges like marking and selections
 * holds also all modified ranges and merges them so a repaint can take its info from here
 *
 * @author Friedrich W. H.  Kossebau
 */
// TODO: split info about ranges from info about dirty ranges into a second class
class OKTETAGUI_EXPORT ByteArrayTableRanges
{
public:
    explicit ByteArrayTableRanges(ByteArrayTableLayout* layout);
    ByteArrayTableRanges() = delete;

    ~ByteArrayTableRanges();

public: // modifcation access
    void setMarking(const AddressRange& marking);
    void setSelectionStart(Address startIndex);
    void setSelectionEnd(Address startIndex);
    void setSelection(const AddressRange& selection);
    /** */
    void setFirstWordSelection(const AddressRange& selection);
    /** */
    void ensureWordSelectionForward(bool Forward);

    /** removes selection with id and returns it */
    AddressRange removeSelection(int id = 0);
    /** removes all but the standard selection and returns true if something changed */
    void removeFurtherSelections();

    /** assumes all added lines to overlap */
    void addChangedOffsetLines(const LineRange& changesLines);

    void addChangedRange(const AddressRange& range);
    void addChangedRange(Address start, Address end);
    void addChangedRange(const CoordRange& range);
    void adaptToChanges(const ArrayChangeMetricsList& changeList, Size oldLength);
    void resetChangedRanges();

    void takeHasSelectionChanged(bool* hasSelectionChanged, bool* selectionChanged);
    void setModified(bool M = true);
    /** removes all ranges */
    void reset();

public: // value access
    int noOfSelections() const;
    Address selectionStart() const;
    Address selectionEnd() const;
    AddressRange selection() const;
    AddressRange firstWordSelection() const;
    Size selectionLength() const;
    AddressRange marking() const;
    bool isModified() const;
    LineRange changedOffsetLines() const;

public: // calculated logic access
    bool hasSelection() const;
    bool hasMarking() const;
    bool selectionStarted() const;
    bool selectionJustStarted() const;
    bool hasFirstWordSelection() const;
    bool selectionIncludes(Address index) const;
    bool markingIncludes(Address index) const;
    // TODO: next three are deprecated
    bool overlapsSelection(Address FirstIndex, Address LastIndex, Address* SI, Address* EI) const;
    bool overlapsMarking(Address FirstIndex, Address LastIndex, Address* SI, Address* EI) const;
//    bool overlapsChanges( int FirstIndex, int LastIndex, int *SI, int *EI ) const;
//    bool overlapsChanges( AddressRange Indizes, AddressRange *ChangedRange ) const;
    bool overlapsChanges(const CoordRange& range, CoordRange* ChangedRange) const;
    const AddressRange* firstOverlappingSelection(const AddressRange& range) const;
    const AddressRange* overlappingMarking(const AddressRange& range) const;

private:
    /** true if something changed */
    bool mModified = false;

    AddressRange mMarking;
    Selection mSelection;
    /** memories first selected word on wordwise selection */
    AddressRange FirstWordSelection;

    /** lines that were added or removed */
    LineRange mChangedOffsetLines;

    CoordRangeList ChangedRanges;

    const QScopedPointer<ByteArrayTableRangesPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ByteArrayTableRanges)
};

inline int ByteArrayTableRanges::noOfSelections()  const { return 1; }

inline Address ByteArrayTableRanges::selectionStart()  const { return mSelection.start(); }
inline Address ByteArrayTableRanges::selectionEnd()    const { return mSelection.end(); }
inline AddressRange ByteArrayTableRanges::selection()  const { return mSelection.range(); }
inline AddressRange ByteArrayTableRanges::firstWordSelection()  const { return FirstWordSelection; }
inline Size ByteArrayTableRanges::selectionLength()    const { return mSelection.range().width(); }
inline AddressRange ByteArrayTableRanges::marking()    const { return mMarking; }
inline bool ByteArrayTableRanges::isModified()         const { return mModified; }
inline LineRange ByteArrayTableRanges::changedOffsetLines() const { return mChangedOffsetLines; }

inline bool ByteArrayTableRanges::hasSelection()             const { return mSelection.isValid(); }
inline bool ByteArrayTableRanges::selectionStarted()         const { return mSelection.started(); }
inline bool ByteArrayTableRanges::selectionJustStarted()     const { return mSelection.justStarted(); }
inline bool ByteArrayTableRanges::hasFirstWordSelection()    const { return FirstWordSelection.isValid(); }
inline bool ByteArrayTableRanges::hasMarking()               const { return mMarking.isValid(); }
inline bool ByteArrayTableRanges::selectionIncludes(Address index) const { return mSelection.range().includes(index); }
inline bool ByteArrayTableRanges::markingIncludes(Address index)   const { return mMarking.includes(index); }

inline void ByteArrayTableRanges::setModified(bool M)           { mModified = M; }

}

#endif
