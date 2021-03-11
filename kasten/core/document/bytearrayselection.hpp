/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSELECTION_HPP
#define KASTEN_BYTEARRAYSELECTION_HPP

// lib
#include "oktetakastencore_export.hpp"
// Kasten core
#include <Kasten/AbstractModelSelection>
// Okteta core
#include <Okteta/AddressRange>

namespace Kasten {

class OKTETAKASTENCORE_EXPORT ByteArraySelection : public AbstractModelSelection
{
public:
    ByteArraySelection();
    ~ByteArraySelection() override;

public:
    void setRange(const Okteta::AddressRange& range);

public:
    Okteta::AddressRange range() const;
    bool isValid() const;

private:
    Okteta::AddressRange mRange;
};

}

#endif
