/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONTOOL_HPP
#define KASTEN_CHARSETCONVERSIONTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/Byte>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}
template <class Key, class T> class QMap;

namespace Kasten {

class ByteArrayView;

/**
 */
class CharsetConversionTool : public AbstractTool
{
    Q_OBJECT

public:
    enum ConversionDirection
    {
        ConvertFrom,
        ConvertTo
    };

    // matching Okteta::ByteArrayComboBox
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        CharCoding = 4,
        Utf8Coding = 5,
        _CodingCount
    };

private:
    static constexpr char ConfigGroupId[] = "CharsetConversionTool";
    static constexpr char OtherCharCodecNameConfigKey[] = "OtherCharCodecName";
    static constexpr char ConversionDirectionConfigKey[] = "ConversionDirection";
    static constexpr char SubstituteMissingCharsConfigKey[] = "SubstituteMissingChars";
    static constexpr char SubstituteByteConfigKey[] = "SubstituteByte";
    static constexpr char SubstituteByteCodingConfigKey[] = "SubstituteByteCoding";

    static constexpr bool DefaultSubstituteMissingChars = false;
    static constexpr Okteta::Byte DefaultSubstituteByte = 0;
    static constexpr ConversionDirection DefaultConversionDirection = ConvertFrom;
    static constexpr CharsetConversionTool::Coding DefaultSubstituteByteCoding = CharsetConversionTool::HexadecimalCoding;

public:
    CharsetConversionTool();
    ~CharsetConversionTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    bool isApplyable() const; // candidate for AbstractTool API

    QString targetCharCodecName() const;
    QString otherCharCodecName() const;
    ConversionDirection conversionDirection() const;
    bool isSubstitutingMissingChars() const;
    Okteta::Byte substituteByte() const;
    Coding substituteByteCoding() const;

public Q_SLOTS: // settings
    void setConversionDirection(int conversionDirection);
    void setOtherCharCodecName(const QString& codecName);
    void setSubstitutingMissingChars(bool isSubstitutingMissingChars);
    void setSubstituteByte(Okteta::Byte byte);
    void setSubstituteByteCoding(Coding coding);

public Q_SLOTS: // actions
    void convertChars();

Q_SIGNALS:
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    void isSubstitutingMissingCharsChanged(bool isSubstitutingMissingChars);
    void targetCharCodecChanged(const QString& charCodingName);
    void conversionDone(bool success, int convertedBytesCount,
                        const QMap<Okteta::Byte, int>& failedPerByteCount);

private Q_SLOTS:
    void onViewCharCodecChanged(const QString& charCodecName);
    void onViewChanged();

private: // settings
    QString mOtherCharCodecName;
    ConversionDirection mConversionDirection = ConvertFrom;
    bool mSubstitutingMissingChars = false;
    Okteta::Byte mSubstituteByte = 0;
    Coding mSubstituteByteCoding = HexadecimalCoding;

private: // sources
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

}

#endif
