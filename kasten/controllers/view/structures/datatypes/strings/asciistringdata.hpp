/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ASCIISTRINGDATA_HPP
#define KASTEN_ASCIISTRINGDATA_HPP

// lib
#include "stringdata.hpp"
// Std
#include <vector>

class AsciiStringData : public StringData
{
public:
    explicit AsciiStringData(StringDataInformation* parent);
    ~AsciiStringData() override;

public: // StringData API
    [[nodiscard]]
    QString typeName() const override;
    [[nodiscard]]
    QString charType() const override;
    [[nodiscard]]
    QString stringValue(int row) const override;
    [[nodiscard]]
    QString completeString() const override;
    [[nodiscard]]
    uint count() const override;
    [[nodiscard]]
    BitCount32 size() const override;
    [[nodiscard]]
    BitCount32 sizeAt(uint i) const override;
    [[nodiscard]]
    qint64 read(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;

private:
    std::vector<quint8> mData;
};

#endif // KASTEN_ASCIISTRINGDATA_HPP
