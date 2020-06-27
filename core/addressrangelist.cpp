/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "addressrangelist.hpp"

namespace Okteta {

AddressRangeList::AddressRangeList() = default;

AddressRangeList::~AddressRangeList() = default;

void AddressRangeList::addAddressRange(const AddressRange& newAddressRange)
{
    if (!newAddressRange.isValid()) {
        return;
    }

    // we try to insert it by ascending indizes
    // if addressRanges are overlapping we combine them
    Iterator firstOverlappingIt = begin();
    for (; firstOverlappingIt != end(); ++firstOverlappingIt) {
        // new addressRange before next addressRange?
        if (newAddressRange.endsBefore((*firstOverlappingIt).nextBeforeStart())) {
            // put the new before it
            insert(firstOverlappingIt, newAddressRange);
            return;
        }

        // does the next addressRange overlap?
        if ((*firstOverlappingIt).isJoinable(newAddressRange)) {
            AddressRange joinedAddressRange(newAddressRange);
            // Start of the joined addressRanges is the smaller one
            joinedAddressRange.extendStartTo((*firstOverlappingIt).start());
            // next we search all the overlapping addressRanges and keep the highest end index
            int joinedEnd = (*firstOverlappingIt).end();
            Iterator lastOverlappingIt = firstOverlappingIt;
            for (++lastOverlappingIt; lastOverlappingIt != end(); ++lastOverlappingIt) {
                if (joinedAddressRange.endsBefore((*lastOverlappingIt).nextBeforeStart())) {
                    break;
                }
                joinedEnd = (*lastOverlappingIt).end();
            }

            // the higher end is the end of the joined addressRange
            joinedAddressRange.extendEndTo(joinedEnd);
            // remove all overlapping addressRanges
            firstOverlappingIt = erase(firstOverlappingIt, lastOverlappingIt);
            // and instead insert the joined one
            insert(firstOverlappingIt, joinedAddressRange);
            return;
        }
    }

    // all others before the new?
    if (firstOverlappingIt == end()) {
        // add it at the end
        append(newAddressRange);
    }
}

void AddressRangeList::addAddressRangeList(const AddressRangeList& addressRangeList)
{
    // TODO: optimize with two parallel iterators
    for (const AddressRange& addressRange : addressRangeList) {
        addAddressRange(addressRange);
    }
}

}
