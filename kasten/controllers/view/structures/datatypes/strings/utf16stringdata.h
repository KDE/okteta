/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef UTF16STRINGDATA_H
#define UTF16STRINGDATA_H

#include "stringdata.h"
#include "structviewpreferences.h"

class Utf16StringData : public StringData
{
public:
    explicit Utf16StringData(StringDataInformation* parent);
    virtual ~Utf16StringData();

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

#endif // UTF16STRINGDATA_H
