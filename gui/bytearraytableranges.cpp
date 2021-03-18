/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraytableranges.hpp"
#include "bytearraytableranges_p.hpp"

// lib
#include "bytearraytablelayout.hpp"
// Okteta core
#include <Okteta/ArrayChangeMetricsList>

namespace Okteta {

ByteArrayTableRanges::ByteArrayTableRanges(ByteArrayTableLayout* layout)
    : d_ptr(new ByteArrayTableRangesPrivate(layout))
{
}

ByteArrayTableRanges::~ByteArrayTableRanges() = default;

void ByteArrayTableRanges::reset()
{
    Q_D(ByteArrayTableRanges);
    mSelection.cancel();
    FirstWordSelection.unset();
    mMarking.unset();
    ChangedRanges.clear();
    d->previousSelection = mSelection;
}

void ByteArrayTableRanges::setMarking(const AddressRange& marking)
{
    if (mMarking == marking) {
        return;
    }

    const bool hadMarking = mMarking.isValid();
    if (hadMarking) {
        addChangedRange(mMarking);
    }

    mMarking = marking;

    const bool hasNewMarking = mMarking.isValid();
    if (hasNewMarking) {
        addChangedRange(mMarking);
    }
}

void ByteArrayTableRanges::removeFurtherSelections()
{
    for (int i = 1; i < noOfSelections(); ++i) {
        removeSelection(i);
    }
}

void ByteArrayTableRanges::setSelection(const AddressRange& selection)
{
    bool Changed = mSelection.isValid();
    if (Changed) {
        addChangedRange(mSelection.range());
    }
    mSelection = selection;
    addChangedRange(mSelection.range());
}

void ByteArrayTableRanges::setSelectionStart(Address startIndex)
{
    bool Changed = mSelection.isValid();
    if (Changed) {
        addChangedRange(mSelection.range());
    }

    mSelection.setStart(startIndex);
}

void ByteArrayTableRanges::setSelectionEnd(Address EndIndex)
{
    AddressRange OldSelection = mSelection.range();
    mSelection.setEnd(EndIndex);

    // TODO: think about rather building a diff of the sections
    if (!OldSelection.isValid()) {
        addChangedRange(mSelection.range());
        return;
    }
    if (!mSelection.isValid()) {
        addChangedRange(OldSelection);
        return;
    }

    if (OldSelection == mSelection.range()) {
        return;
    }
    Address CS;
    Address CE;
    // changes at the end?
    if (mSelection.start() == OldSelection.start()) {
        CS = OldSelection.nextBehindEnd();
        CE = mSelection.end();
        if (CE < CS) {
            CS = mSelection.nextBehindEnd();
            CE = OldSelection.end();
        }
    }
    // changes at the start?
    else if (mSelection.end() == OldSelection.end()) {
        CS = OldSelection.start();
        CE = mSelection.nextBeforeStart();
        if (CE < CS) {
            CS = mSelection.start();
            CE = OldSelection.nextBeforeStart();
        }
    }
    // change over the anchor
    else {
        CS = OldSelection.start();
        CE = mSelection.end();
        if (CE < CS) {
            CS = mSelection.start();
            CE = OldSelection.end();
        }
    }
    AddressRange C(CS, CE);

    bool Changed = C.isValid();
    if (Changed) {
        addChangedRange(C);
    }
    return;
}

AddressRange ByteArrayTableRanges::removeSelection(int id)
{
    if (id > 0) {
        return AddressRange();
    }

    AddressRange range = mSelection.range();
    bool Changed = range.isValid();
    if (Changed) {
        addChangedRange(range);
    }

    mSelection.cancel();
    FirstWordSelection.unset();

    return range;
}

bool ByteArrayTableRanges::overlapsSelection(Address FirstIndex, Address LastIndex, Address* startIndex, Address* endIndex) const
{
    if (mSelection.range().overlaps(AddressRange(FirstIndex, LastIndex))) {
        *startIndex = mSelection.start();
        *endIndex = mSelection.end();
        return true;
    }
    return false;
}

bool ByteArrayTableRanges::overlapsMarking(Address FirstIndex, Address LastIndex, Address* startIndex, Address* endIndex) const
{
    if (mMarking.overlaps(AddressRange(FirstIndex, LastIndex))) {
        *startIndex = mMarking.start();
        *endIndex = mMarking.end();
        return true;
    }
    return false;
}

const AddressRange* ByteArrayTableRanges::firstOverlappingSelection(const AddressRange& Range) const
{
    return mSelection.range().overlaps(Range) ? &mSelection.range() : nullptr;
}

const AddressRange* ByteArrayTableRanges::overlappingMarking(const AddressRange& Range) const
{
    return mMarking.overlaps(Range) ? &mMarking : nullptr;
}

/*
bool ByteArrayTableRanges::overlapsChanges( Address FirstIndex, Address LastIndex, Address* startIndex, Address* endIndex ) const
{
    for( CoordRangeList::const_iterator S=ChangedRanges.begin(); S!=ChangedRanges.end(); ++S )
    {
        if( (*S).overlaps(KBuff(FirstIndex,LastIndex)) )
        {
            *startIndex = (*S).start();
            *endIndex = (*S).end();
            return true;
        }
    }

    return false;
}

bool ByteArrayTableRanges::overlapsChanges( AddressRange Indizes, AddressRange *ChangedRange ) const
{
    for( AddressRangeList::const_iterator S=ChangedRanges.begin(); S!=ChangedRanges.end(); ++S )
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

bool ByteArrayTableRanges::overlapsChanges(const CoordRange& Range, CoordRange* ChangedRange) const
{
    // TODO: add a lastusedrange pointer for quicker access
    return std::any_of(ChangedRanges.begin(), ChangedRanges.end(),
                       [Range, ChangedRange](const CoordRange& changedRange) mutable {
        if (changedRange.overlaps(Range)) {
            *ChangedRange = changedRange;
            return true;
        }
        return false;
    });
}

void ByteArrayTableRanges::addChangedOffsetLines(const LineRange& changedLines)
{
    if (mChangedOffsetLines.isEmpty()) {
        mChangedOffsetLines = changedLines;
        mModified = true;
    } else {
        mChangedOffsetLines.extendTo(changedLines);
    }
}

void ByteArrayTableRanges::addChangedRange(Address startIndex, Address endIndex)
{
    addChangedRange(AddressRange(startIndex, endIndex));
}

void ByteArrayTableRanges::addChangedRange(const AddressRange& range)
{
    Q_D(ByteArrayTableRanges);
// qCDebug(LOG_OKTETA_GUI) << "adding change range "<<S.start()<<","<<S.end();
    addChangedRange(d->layout->coordRangeOfIndizes(range));
}

void ByteArrayTableRanges::addChangedRange(const CoordRange& range)
{
    ChangedRanges.addCoordRange(range);
// qCDebug(LOG_OKTETA_GUI) << "as range "<<NewRange.start().pos()<<","<<NewRange.start().line()<<"-"
// <<NewRange.end().pos()<<","<<NewRange.end().line()<<endl;

    mModified = true;
}

void ByteArrayTableRanges::resetChangedRanges()
{
    mChangedOffsetLines.unset();
    ChangedRanges.clear();
    mModified = false;
}

void ByteArrayTableRanges::takeHasSelectionChanged(bool* hasSelectionChanged, bool* selectionChanged)
{
    Q_D(ByteArrayTableRanges);

    const bool hadSelection = d->previousSelection.isValid();
    const bool hasSelection = mSelection.isValid();
    *hasSelectionChanged = (hadSelection != hasSelection);

    *selectionChanged = (d->previousSelection != mSelection);

    if (*selectionChanged) {
        d->previousSelection = mSelection;
    }
}

void ByteArrayTableRanges::setFirstWordSelection(const AddressRange& range)
{
    FirstWordSelection = range;
    setSelection(FirstWordSelection);
}

void ByteArrayTableRanges::ensureWordSelectionForward(bool Forward)
{
    // in the anchor not on the right side?
    if (mSelection.isForward() != Forward) {
        setSelectionEnd(Forward ? FirstWordSelection.start() : FirstWordSelection.nextBehindEnd());

        mSelection.setForward(Forward);
    }
}

void ByteArrayTableRanges::adaptToChanges(const ArrayChangeMetricsList& changeList, Size oldLength)
{
    for (const ArrayChangeMetrics& change : changeList) {
        // TODO: change parameters to ArrayChangeMetrics
        switch (change.type())
        {
        case ArrayChangeMetrics::Replacement:
        {
            oldLength += change.lengthChange();
            const Address offset = change.offset();
            const Size diff = change.lengthChange();
            const Address behindLast = (diff == 0) ? offset + change.insertLength() :
                                       (diff < 0) ?  oldLength - diff :
                                                     oldLength;
            addChangedRange(offset, behindLast - 1);

            if (mSelection.isValid()) {
                mSelection.adaptToReplacement(offset, change.removeLength(), change.insertLength());
            }
            if (mMarking.isValid()) {
                mMarking.adaptToReplacement(offset, change.removeLength(), change.insertLength());
            }
            break;
        }
        case ArrayChangeMetrics::Swapping:
            addChangedRange(change.offset(), change.secondEnd());

            if (mSelection.isValid()) {
                mSelection.adaptToSwap(change.offset(), change.secondStart(), change.secondLength());
            }
        // TODO:
//             if( mMarking.isValid() )
//                 mMarking.adaptToSwap( change.offset(), change.secondStart(), change.secondLength() );
        default:
            ;
        }
    }
}

}
