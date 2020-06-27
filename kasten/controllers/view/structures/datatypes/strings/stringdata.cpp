/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringdata.hpp"
#include "stringdatainformation.hpp"

StringData::StringData(StringDataInformation* parent)
    : mParent(parent)
    , mMode(None)
    , mLittleEndian(true)
    , mEofReached(false)
{
    // default to zero terminated strings
    mLength.maxBytes = 0;
}

StringData::~StringData() = default;

void StringData::setTerminationCodePoint(uint term)
{
    if (term > UNICODE_MAX) {
        mParent->logError() << "Attempting to set termination code point which is larger"
            " than 0x10ffff (max unicode):" << term;
        return;
    }
    mTerminationCodePoint = term;
    mMode |= Sequence;
}

void StringData::setLittleEndian(bool littleEndian)
{
    mLittleEndian = littleEndian;
}
