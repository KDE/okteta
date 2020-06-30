/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractvalue8editor.hpp"

// Okteta core
#include <Okteta/ValueCodec>
// C++
#include <limits>

AbstractValue8Editor::AbstractValue8Editor(Okteta::ValueCoding valueCoding, QWidget* parent)
    : QSpinBox(parent)
{
    const quint8 int8Max = std::numeric_limits<quint8>::max();
    const quint8 int8Min = std::numeric_limits<quint8>::min();

    setRange(int8Min, int8Max);

    mValueCodec = Okteta::ValueCodec::createCodec(valueCoding);
}

AbstractValue8Editor::~AbstractValue8Editor()
{
    delete mValueCodec;
}

QString AbstractValue8Editor::textFromValue(int value) const
{
    QString result;

    mValueCodec->encodeShort(&result, 0, (Okteta::Byte)value);

    return result;
}

int AbstractValue8Editor::valueFromText(const QString& text) const
{
    Okteta::Byte byte;
    mValueCodec->decode(&byte, text);

    return byte;
}

QValidator::State AbstractValue8Editor::validate(QString& text, int& pos) const
{
    Q_UNUSED(pos);

    Okteta::Byte byte;
    const int textLength = text.length();
    const int usedChars = mValueCodec->decode(&byte, text);

    const bool isCompleteTextUsed = (textLength == usedChars);

    return isCompleteTextUsed ? QValidator::Acceptable : QValidator::Invalid;
}
