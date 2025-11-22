/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF32STRINGDATA_HPP
#define KASTEN_UTF32STRINGDATA_HPP

// lib
#include "stringdata.hpp"
// Std
#include <vector>

class Utf32StringData : public StringData
{
public:
    explicit Utf32StringData(StringDataInformation* parent);
    ~Utf32StringData() override;

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
    std::vector<quint32> mCodePoints;
    int mNonBMPCount = 0;
};

#endif // KASTEN_UTF32STRINGDATA_HPP
