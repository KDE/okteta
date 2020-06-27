/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF16STRINGDATA_HPP
#define KASTEN_UTF16STRINGDATA_HPP

#include "stringdata.hpp"

#include <QVector>

class Utf16StringData : public StringData
{
public:
    explicit Utf16StringData(StringDataInformation* parent);
    ~Utf16StringData() override;

    QString charType() const override;
    QString completeString(bool skipInvalid = false) const override;
    uint count() const override;
    qint64 read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;
    BitCount32 size() const override;
    BitCount32 sizeAt(uint i) const override;
    QString stringValue(int row) const override;
    QString typeName() const override;

private:
    QVector<quint32> mCodePoints;
    int mNonBMPCount = 0;
};

#endif // KASTEN_UTF16STRINGDATA_HPP
