/*
    SPDX-FileCopyrightText: 2008, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "operandbytearrayfilterparameterset.hpp"

// libconfigentries
#include <bytearraycodingconfigentry.hpp>
// KF
#include <KConfigGroup>

template <>
inline OperandByteArrayFilterParameterSet::Coding
KConfigGroup::readEntry(const char *key,
                        const OperandByteArrayFilterParameterSet::Coding &defaultValue) const
{
    return static_cast<OperandByteArrayFilterParameterSet::Coding>(readEntry(key, static_cast<Kasten::ByteArrayCoding>(defaultValue)));
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const OperandByteArrayFilterParameterSet::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    writeEntry(key, static_cast<Kasten::ByteArrayCoding>(value), flags);
}

const QByteArray OperandByteArrayFilterParameterSet::DefaultOperand = QByteArray();


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
