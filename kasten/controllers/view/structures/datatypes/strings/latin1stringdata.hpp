/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_LATIN1STRINGDATA_HPP
#define KASTEN_LATIN1STRINGDATA_HPP

// lib
#include "stringdata.hpp"
// Std
#include <vector>

class Latin1StringData : public StringData
{
public:
    explicit Latin1StringData(StringDataInformation* parent);
    ~Latin1StringData() override;

public: // StringData API
    QString typeName() const override;
    QString charType() const override;
    QString stringValue(int row) const override;
    QString completeString() const override;
    uint count() const override;
    BitCount32 size() const override;
    BitCount32 sizeAt(uint i) const override;
    qint64 read(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;

private:
    std::vector<quint8> mData;
};

#endif // KASTEN_LATIN1STRINGDATA_HPP
