/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ADDRESSRANGELIST_HPP
#define OKTETA_ADDRESSRANGELIST_HPP

// lib
#include "addressrange.hpp"
// Qt
#include <QLinkedList>

namespace Okteta {

/**
   @author Friedrich W. H.  Kossebau
 */
class AddressRangeList : public QLinkedList<AddressRange>
{
public:
    AddressRangeList();

    ~AddressRangeList();

public:
    void addAddressRange(const AddressRange& addressRange);
    void addAddressRangeList(const AddressRangeList& addressRangeList);
};

}

#endif
