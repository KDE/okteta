/*
    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBITFIELDDATAINFORMATION_HPP
#define KASTEN_ABSTRACTBITFIELDDATAINFORMATION_HPP

// lib
#include <primitivedatainformation.hpp>
#include <allprimitivetypes.hpp>
// Std
#include <algorithm>

class AbstractBitfieldDataInformation : public PrimitiveDataInformation
{
public:
    enum class Type {
        Signed,
        Unsigned,
        Boolean
    };

private:
    static constexpr quint8 MaxWidth = 64; // maximum width is 64 bits

protected:
    AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d);

public:
    AbstractBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = nullptr);
    ~AbstractBitfieldDataInformation() override;

public: // API to implement
    [[nodiscard]]
    virtual Type bitfieldType() const = 0;

public: // PrimitiveDataInformation API
    [[nodiscard]]
    PrimitiveDataType type() const override;
    [[nodiscard]]
    AllPrimitiveTypes value() const override;

    void setValue(AllPrimitiveTypes newVal) override;

public: // DataInformation API
    [[nodiscard]]
    Qt::ItemFlags flags(int column, bool fileLoaded) const override;

    [[nodiscard]]
    QString sizeString() const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input,
                    Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset) override;
    [[nodiscard]]
    bool setData(const QVariant& valueVariant, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isBitfield() const override;

public:
    [[nodiscard]]
    BitCount32 width() const;
    void setWidth(BitCount32 newWidth);
    [[nodiscard]]
    quint64 mask() const;

protected: // API to implement
    [[nodiscard]]
    virtual AllPrimitiveTypes fromVariant(const QVariant& variant, bool* ok) const;

private: // DataInformation API
    [[nodiscard]]
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
     * for (int i = 0; i < width(); ++i)
     *     ret |= 1 << i;
     * return ret;
     */
    if (Q_UNLIKELY(mWidth == MaxWidth)) {
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
    Q_ASSERT(newWidth > 0 && newWidth <= MaxWidth);
    mWidth = std::min(newWidth, static_cast<BitCount32>(MaxWidth));
}

#endif /* KASTEN_ABSTRACTBITFIELDDATAINFORMATION_HPP */
