/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF8STRINGDATA_HPP
#define KASTEN_UTF8STRINGDATA_HPP

// lib
#include "stringdata.hpp"
// Qt
#include <QHash>
// Std
#include <vector>

class Utf8StringData : public StringData
{
public:
    explicit Utf8StringData(StringDataInformation* parent);
    ~Utf8StringData() override;

    qint64 read(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;
    BitCount32 sizeAt(uint i) const override;
    BitCount32 size() const override;
    QString completeString() const override;
    QString stringValue(int row) const override;
    QString charType() const override;
    uint count() const override;
    QString typeName() const override;

private:
    QHash<int, quint8> mErrorIndices;
    std::vector<quint32> mCodePoints;
    uint mOneByteCount = 0;
    uint mTwoByteCount = 0;
    uint mThreeByteCount = 0;
    uint mFourByteCount = 0;
    uint mNonBMPCount = 0;
};

#endif // KASTEN_UTF8STRINGDATA_HPP
