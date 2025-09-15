/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONTOOL_HPP
#define KASTEN_CHARSETCONVERSIONTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/Byte>
// Std
#include <map>

namespace Okteta {
class AbstractByteArrayModel;
}

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
    static inline constexpr char ConfigGroupId[] = "CharsetConversionTool";
    static inline constexpr char OtherCharCodecNameConfigKey[] = "OtherCharCodecName";
    static inline constexpr char ConversionDirectionConfigKey[] = "ConversionDirection";
    static inline constexpr char SubstituteMissingCharsConfigKey[] = "SubstituteMissingChars";
    static inline constexpr char SubstituteByteConfigKey[] = "SubstituteByte";
    static inline constexpr char SubstituteByteCodingConfigKey[] = "SubstituteByteCoding";

    static inline constexpr bool DefaultSubstituteMissingChars = false;
    static inline constexpr Okteta::Byte DefaultSubstituteByte = 0;
    static inline constexpr ConversionDirection DefaultConversionDirection = ConvertFrom;
    static inline constexpr Coding DefaultSubstituteByteCoding = HexadecimalCoding;

public:
    CharsetConversionTool();
    ~CharsetConversionTool() override;

public: // AbstractTool API
    [[nodiscard]]
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    [[nodiscard]]
    bool isApplyable() const; // candidate for AbstractTool API

    [[nodiscard]]
    QString targetCharCodecName() const;
    [[nodiscard]]
    QString otherCharCodecName() const;
    [[nodiscard]]
    ConversionDirection conversionDirection() const;
    [[nodiscard]]
    bool isSubstitutingMissingChars() const;
    [[nodiscard]]
    Okteta::Byte substituteByte() const;
    [[nodiscard]]
    Coding substituteByteCoding() const;

    /// AbstractTool API candidate
    /// most outer model which is used
    /// what if multiple models are touched?
    [[nodiscard]]
    AbstractModel* targetModel() const;

public Q_SLOTS: // settings
    void setConversionDirection(int conversionDirection);
    void setOtherCharCodecName(const QString& codecName);
    void setSubstitutingMissingChars(bool isSubstitutingMissingChars);
    void setSubstituteByte(Okteta::Byte byte);
    void setSubstituteByteCoding(Kasten::CharsetConversionTool::Coding coding);

public Q_SLOTS: // actions
    void convertChars();

Q_SIGNALS:
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    void isSubstitutingMissingCharsChanged(bool isSubstitutingMissingChars);
    void targetCharCodecChanged(const QString& charCodingName);
    void conversionDone(bool success, int convertedBytesCount,
                        const std::map<Okteta::Byte, int>* failedPerByteCount);

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
