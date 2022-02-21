/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "operandbytearrayfilterparameterset.hpp"

// KF
#include <KConfigGroup>
// Std
#include <array>
#include <algorithm>
#include <iterator>

static constexpr int codingCount =
    static_cast<int>(OperandByteArrayFilterParameterSet::_CodingCount);
static const std::array<QString, codingCount> codingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Decimal"),
    QStringLiteral("Octal"),
    QStringLiteral("Binary"),
    QStringLiteral("Char"),
    QStringLiteral("UTF-8"),
};


template <>
inline OperandByteArrayFilterParameterSet::Coding
KConfigGroup::readEntry(const char *key,
                        const OperandByteArrayFilterParameterSet::Coding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(codingConfigValueList.cbegin(), codingConfigValueList.cend(), entry);
    if (it == codingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(codingConfigValueList.cbegin(), it);
    return static_cast<OperandByteArrayFilterParameterSet::Coding>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const OperandByteArrayFilterParameterSet::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    QString configValue;
    if (value == OperandByteArrayFilterParameterSet::InvalidCoding) {
        configValue = QStringLiteral("Invalid");
    } else {
        const int listIndex = static_cast<int>(value);
        configValue = codingConfigValueList[listIndex];
    }
    writeEntry(key, configValue, flags);
}

static const QByteArray DefaultOperand = QByteArray();
static constexpr OperandByteArrayFilterParameterSet::Coding DefaultOperandFormat = OperandByteArrayFilterParameterSet::HexadecimalCoding;
static constexpr bool DefaultAlignAtEnd = false;

static constexpr char OperandConfigKey[] = "Operand";
static constexpr char ByteOrderConfigKey[] = "OperandFormat";
static constexpr char AlignAtEndConfigKey[] = "AlignAtEnd";

OperandByteArrayFilterParameterSet::OperandByteArrayFilterParameterSet() = default;

OperandByteArrayFilterParameterSet::~OperandByteArrayFilterParameterSet() = default;

const char* OperandByteArrayFilterParameterSet::id() const { return "Operand"; }

QByteArray OperandByteArrayFilterParameterSet::operand() const { return mOperand; }
OperandByteArrayFilterParameterSet::Coding OperandByteArrayFilterParameterSet::operandFormat() const { return mOperandFormat; }
bool OperandByteArrayFilterParameterSet::alignAtEnd() const { return mAlignAtEnd; }

void OperandByteArrayFilterParameterSet::setOperand(const QByteArray& operand) { mOperand = operand; }
void OperandByteArrayFilterParameterSet::setOperandFormat(Coding operandFormat) { mOperandFormat = operandFormat; }
void OperandByteArrayFilterParameterSet::setAlignAtEnd(bool alignAtEnd) { mAlignAtEnd = alignAtEnd; }

void OperandByteArrayFilterParameterSet::loadConfig(const KConfigGroup& configGroup)
{
    mOperand = configGroup.readEntry(OperandConfigKey, DefaultOperand);
    mOperandFormat = configGroup.readEntry(ByteOrderConfigKey, DefaultOperandFormat);
    mAlignAtEnd = configGroup.readEntry(AlignAtEndConfigKey, DefaultAlignAtEnd);
}

void OperandByteArrayFilterParameterSet::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(OperandConfigKey, mOperand);
    configGroup.writeEntry(ByteOrderConfigKey, mOperandFormat);
    configGroup.writeEntry(AlignAtEndConfigKey, mAlignAtEnd);
}
