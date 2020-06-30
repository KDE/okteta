/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sintspinbox.hpp"

// Qt
#include <QLineEdit>

QValidator::State SIntSpinBox::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos);

    QValidator::State result;

    if (input.isEmpty()
        || (mMinimum < 0 && input == QLatin1String("-"))) {
        mValue = 0;
        result = QValidator::Intermediate;
    } else {
        bool ok;
        qint64 newValue = input.toLongLong(&ok);
        if (!ok
            || (newValue > mMaximum)
            || (newValue < mMinimum)) {
            result = QValidator::Invalid;
        } else {
            mValue = newValue;
            result = QValidator::Acceptable;
        }
    }

    return result;
}

void SIntSpinBox::fixup(QString& input) const
{
    Q_UNUSED(input);

    // TODO: what can be done here? remove localized stuff?
}

void SIntSpinBox::stepBy(int steps)
{
    if (steps == 0) {
        return;
    }

    if (steps > 0) {
        const qint64 left = mMaximum - mValue;
        mValue = (static_cast<qint64>(steps) > left) ? mMaximum : mValue + steps;
    } else {
        const qint64 left = mValue - mMinimum;
        mValue = (static_cast<qint64>(-steps) > left) ? mMinimum : mValue + steps;
    }

    updateEditLine();
}

QAbstractSpinBox::StepEnabled SIntSpinBox::stepEnabled() const
{
    StepEnabled result;

    if (mValue > mMinimum) {
        result |= StepDownEnabled;
    }
    if (mValue < mMaximum) {
        result |= StepUpEnabled;
    }

    return result;
}

void SIntSpinBox::updateEditLine() const
{
    const QString text = QString::number(mValue, mBase);
    lineEdit()->setText(text);
}
