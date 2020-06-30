/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBITFIELDDATAINFORMATION_HPP
#define KASTEN_ABSTRACTBITFIELDDATAINFORMATION_HPP

#include "../primitivedatainformation.hpp"
#include "../../../allprimitivetypes.hpp"

class AbstractBitfieldDataInformation : public PrimitiveDataInformation
{
public:
    enum class Type {
        Signed,
        Unsigned,
        Boolean
    };

    AbstractBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = nullptr);
    ~AbstractBitfieldDataInformation() override;

    BitCount32 width() const;
    void setWidth(BitCount32 newWidth);
    BitCount32 size() const override;
    quint64 mask() const;
    AllPrimitiveTypes value() const override;
    void setValue(AllPrimitiveTypes newVal) override;
    PrimitiveDataType type() const override;
    QString sizeString() const override;
    bool isBitfield() const override;
    virtual Type bitfieldType() const = 0;
    Qt::ItemFlags flags(int column, bool fileLoaded) const override;
    qint64 readData(Okteta::AbstractByteArrayModel* input,
                    Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset) override;
    bool setData(const QVariant& valueVariant, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

protected:
    AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d);
    virtual AllPrimitiveTypes fromVariant(const QVariant& variant, bool* ok) const;

private:
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

protected:
    AllPrimitiveTypes mValue = 0;
    quint8 mWidth; // cannot be more than 64 since a quint64 is used for storage
};

inline quint64 AbstractBitfieldDataInformation::mask() const
{
    /* same as:
     *
     * quint64 ret = 0;
     * for (int i = 0; i < width(); i++)
     *     ret |= 1 << i;
     * return ret;
     */
    if (Q_UNLIKELY(mWidth == 64u)) {
        return ~quint64(0);
    }
    return (quint64(1) << mWidth) - 1;
}

inline BitCount32 AbstractBitfieldDataInformation::width() const
{
    return mWidth;
}

inline void AbstractBitfieldDataInformation::setWidth(BitCount32 newWidth)
{
    Q_ASSERT(newWidth > 0 && newWidth <= 64);
    mWidth = qMin(newWidth, BitCount32(64u)); // maximum width is 64 bits
}

#endif /* KASTEN_ABSTRACTBITFIELDDATAINFORMATION_HPP */
