/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "operandbytearrayfilterparameterset.hpp"

OperandByteArrayFilterParameterSet::OperandByteArrayFilterParameterSet() = default;

OperandByteArrayFilterParameterSet::~OperandByteArrayFilterParameterSet() = default;

const char* OperandByteArrayFilterParameterSet::id() const { return "Operand"; }

QByteArray OperandByteArrayFilterParameterSet::operand() const { return mOperand; }
int OperandByteArrayFilterParameterSet::operandFormat() const { return mOperandFormat; }
bool OperandByteArrayFilterParameterSet::alignAtEnd() const { return mAlignAtEnd; }

void OperandByteArrayFilterParameterSet::setOperand(const QByteArray& operand) { mOperand = operand; }
void OperandByteArrayFilterParameterSet::setOperandFormat(int operandFormat) { mOperandFormat = operandFormat; }
void OperandByteArrayFilterParameterSet::setAlignAtEnd(bool alignAtEnd) { mAlignAtEnd = alignAtEnd; }
