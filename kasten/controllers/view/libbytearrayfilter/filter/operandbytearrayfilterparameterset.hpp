/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OPERANDBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_OPERANDBYTEARRAYFILTERPARAMETERSET_HPP

// lib
#include "abstractbytearrayfilterparameterset.hpp"
// Qt
#include <QByteArray>

class KConfigGroup;

class OperandByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
public:
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
    static inline constexpr OperandByteArrayFilterParameterSet::Coding DefaultOperandFormat = OperandByteArrayFilterParameterSet::HexadecimalCoding;
    static inline constexpr bool DefaultAlignAtEnd = false;

    static const QByteArray DefaultOperand;

    static inline constexpr char OperandConfigKey[] = "Operand";
    static inline constexpr char ByteOrderConfigKey[] = "OperandFormat";
    static inline constexpr char AlignAtEndConfigKey[] = "AlignAtEnd";

public:
    OperandByteArrayFilterParameterSet();
    ~OperandByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    [[nodiscard]]
    const char* id() const override;

public:
    void setOperand(const QByteArray& operand);
    void setOperandFormat(Coding operandFormat);
    void setAlignAtEnd(bool alignAtEnd);

public:
    [[nodiscard]]
    QByteArray operand() const;
    [[nodiscard]]
    Coding operandFormat() const;
    [[nodiscard]]
    bool alignAtEnd() const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

private:
    QByteArray mOperand;
    Coding mOperandFormat = HexadecimalCoding;
    bool mAlignAtEnd = false;
};

#endif
