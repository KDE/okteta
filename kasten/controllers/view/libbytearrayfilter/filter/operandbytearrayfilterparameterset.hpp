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

class OperandByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
public:
    OperandByteArrayFilterParameterSet();
    ~OperandByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    const char* id() const override;

public:
    void setOperand(const QByteArray& operand);
    void setOperandFormat(int operandFormat);
    void setAlignAtEnd(bool alignAtEnd);

public:
    QByteArray operand() const;
    int operandFormat() const;
    bool alignAtEnd() const;

private:
    QByteArray mOperand;
    int mOperandFormat = 0;
    bool mAlignAtEnd = false;
};

#endif
