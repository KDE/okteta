/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Alex Richardson <alex.richardson@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef UTF32STRINGDATA_H
#define UTF32STRINGDATA_H

#include "stringdata.h"

#include <QVector>

class Utf32StringData : public StringData
{
public:
    explicit Utf32StringData(StringDataInformation* parent);
    ~Utf32StringData() override;

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

#endif // UTF32STRINGDATA_H
