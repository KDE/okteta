/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uintspinbox.hpp"

// Qt
#include <QLineEdit>

QValidator::State UIntSpinBox::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos);

    QValidator::State result;

    if (input.isEmpty()
        || mPrefix.startsWith(input)) {
        mValue = 0;
        result = QValidator::Intermediate;
    } else {
        bool ok;
        quint64 newValue = input.toULongLong(&ok, mBase);
        if (!ok
            || (newValue > mMaximum)) {
            result = QValidator::Invalid;
        } else {
            mValue = newValue;
            result = QValidator::Acceptable;
        }
    }

    return result;
}

void UIntSpinBox::fixup(QString& input) const
{
    Q_UNUSED(input);

    // TODO: what can be done here? remove localized stuff?
}

void UIntSpinBox::stepBy(int steps)
{
    if (steps == 0) {
        return;
    }

    if (steps > 0) {
        const quint64 left = mMaximum - mValue;
        mValue = (static_cast<quint64>(steps) > left) ? mMaximum : mValue + steps;
    } else {
        const quint64 left = mValue;
        mValue = (static_cast<quint64>(-steps) > left) ? 0 : mValue + steps;
    }

    updateEditLine();
}

QAbstractSpinBox::StepEnabled UIntSpinBox::stepEnabled() const
{
    StepEnabled result;

    if (mValue > 0) {
        result |= StepDownEnabled;
    }
    if (mValue < mMaximum) {
        result |= StepUpEnabled;
    }

    return result;
}

void UIntSpinBox::updateEditLine() const
{
    const QString text = mPrefix + QString::number(mValue, mBase);
    lineEdit()->setText(text);
}
