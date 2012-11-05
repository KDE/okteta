/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef ABSTRACTBITFIELDDATAINFORMATION_H_
#define ABSTRACTBITFIELDDATAINFORMATION_H_

#include "../primitivedatainformation.h"
#include "../../../allprimitivetypes.h"

class AbstractBitfieldDataInformation : public PrimitiveDataInformation
{
public:
    AbstractBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = 0);
    virtual ~AbstractBitfieldDataInformation();

    BitCount32 width() const;
    void setWidth(BitCount32 newWidth);
    virtual BitCount32 size() const;
    quint64 mask() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);
    virtual PrimitiveDataType type() const;
    virtual QString sizeString() const;
    virtual bool isBitfield() const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded) const;
    virtual qint64 readData(Okteta::AbstractByteArrayModel *input,
            Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset);
    bool setData(const QVariant& valueVariant, Okteta::AbstractByteArrayModel *out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);

protected:
    AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d);
    virtual AllPrimitiveTypes fromVariant(const QVariant& variant, bool* ok) const;
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const;
protected:
    AllPrimitiveTypes mValue;
    quint8 mWidth; //cannot be more than 64 since a quint64 is used for storage
};

inline Qt::ItemFlags AbstractBitfieldDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == (int) DataInformation::ColumnValue && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

inline quint64 AbstractBitfieldDataInformation::mask() const
{
    /* same as:
     *
     * quint64 ret = 0;
     * for (int i = 0; i < width(); i++)
     *     ret |= 1 << i;
     * return ret;
     */
    if (Q_UNLIKELY(mWidth == 64u))
        return ~quint64(0);
    return (quint64(1) << mWidth) - 1;
}

inline BitCount32 AbstractBitfieldDataInformation::size() const
{
    return mWidth;
}

inline BitCount32 AbstractBitfieldDataInformation::width() const
{
    return mWidth;
}

inline void AbstractBitfieldDataInformation::setWidth(BitCount32 newWidth)
{
    Q_ASSERT(newWidth > 0 && newWidth <= 64);
    mWidth = qMin(newWidth, BitCount32(64u)); //maximum width is 64 bits
}

inline bool AbstractBitfieldDataInformation::isBitfield() const
{
    return true;
}

#endif /* ABSTRACTBITFIELDDATAINFORMATION_H_ */
