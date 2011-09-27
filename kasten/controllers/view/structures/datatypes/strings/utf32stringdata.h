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


class Utf32StringData : public StringData
{
public:
    explicit Utf32StringData(StringDataInformation* parent);
    virtual ~Utf32StringData();

    virtual QString charType() const;
    virtual QString completeString(bool skipInvalid = false) const;
    int count() const;
    virtual qint64 read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining);
    virtual BitCount32 size() const;
    virtual BitCount32 sizeAt(int i) const;
    virtual QString stringValue(int row) const;
    virtual QString typeName() const;
private:
    QVector<quint32> mCodePoints;
    int mNonBMPCount;
};

#endif // UTF32STRINGDATA_H
