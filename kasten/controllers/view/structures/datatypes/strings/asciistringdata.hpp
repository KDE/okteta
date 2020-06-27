/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ASCIISTRINGDATA_HPP
#define KASTEN_ASCIISTRINGDATA_HPP

#include "stringdata.hpp"

#include <QVector>

class AsciiStringData : public StringData
{
public:
    explicit AsciiStringData(StringDataInformation* parent);
    ~AsciiStringData() override;

    qint64 read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;
    BitCount32 sizeAt(uint i) const override;
    BitCount32 size() const override;
    QString completeString(bool skipInvalid = false) const override;
    QString stringValue(int row) const override;
    QString charType() const override;
    uint count() const override;
    QString typeName() const override;

private:
    QVector<quint8> mData;
};

#endif // KASTEN_ASCIISTRINGDATA_HPP
