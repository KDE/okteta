/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "addressvalidator.hpp"

// lib
#include <logging.hpp>
// Okteta core
#include <Okteta/ValueCodec>
// Qt
#include <QString>
#include <QJSEngine>
#include <QJSValue>

namespace Okteta {

AddressValidator::AddressValidator(QObject* parent, Coding codecId)
    : QValidator(parent)
{
    setCodec(codecId);
}

AddressValidator::~AddressValidator() = default;

void AddressValidator::setCodec(Coding codecId)
{
    if (codecId == mCodecId) {
        return;
    }

    mCodecId = codecId;

    mValueCodec = ValueCodec::createCodec((Okteta::ValueCoding)mCodecId);
}

const QRegularExpression AddressValidator::expressionRegex =
    QRegularExpression(QStringLiteral("\\A[0-9a-fx\\+/\\s\\-\\*]+\\Z"),
            QRegularExpression::CaseInsensitiveOption); // FIXME this is way too simple, only there to test

QValidator::State AddressValidator::validate(QString& string, int& pos) const
{
    Q_UNUSED(pos)

    State result = QValidator::Acceptable;
    if (mCodecId == ExpressionCoding) {
        string = string.trimmed();
        if (!expressionRegex.match(string).hasMatch()) {
            result = QValidator::Invalid;
        }
        // only prefix has been typed:
        if (string == QLatin1String("+")
            || string == QLatin1String("-")
            || string.endsWith(QLatin1Char('x'))) { // 0x at end
            result = QValidator::Intermediate;
        }
    } else {
        const int stringLength = string.length();
        for (int i = 0; i < stringLength; ++i) {
            const QChar c = string.at(i);
            if (!mValueCodec->isValidDigit(c.toLatin1()) && !c.isSpace()) {
                result = QValidator::Invalid;
                break;
            }
        }
    }
    if (string.isEmpty()) {
        result = QValidator::Intermediate;
    }
    return result;
}

Address AddressValidator::toAddress(const QString& string, AddressType* addressType) const
{
    Address address;

    QString expression = string.trimmed();

    if (addressType) {
        const AddressType type =
            expression.startsWith(QLatin1Char('+')) ? RelativeForwards :
            expression.startsWith(QLatin1Char('-')) ? RelativeBackwards :
            /* else */                                AbsoluteAddress;

        if (type != AbsoluteAddress) {
            expression.remove(0, 1);
        }

        *addressType = type;
    }

    if (mCodecId == ExpressionCoding) {
        QJSEngine evaluator;
        QJSValue value = evaluator.evaluate(expression);
        address = value.toInt();
        qCDebug(LOG_KASTEN_OKTETA_GUI) << "expression " << expression << " evaluated to: " << address;

        if (value.isError()) {
            qCWarning(LOG_KASTEN_OKTETA_GUI) << "evaluation error: " << value.toString();
            if (addressType) {
                *addressType = InvalidAddressType;
            }
        }
    } else {
        const int base = (mCodecId == HexadecimalCoding) ? 16 : (mCodecId == DecimalCoding) ? 10 : 8;
        address = expression.toInt(nullptr, base);
    }

    return address;
}

QString AddressValidator::toString(Address address, AddressType addressType) const
{
    // ExpressionCoding just uses base 10 so no need to adjust this code
    const int base = (mCodecId == HexadecimalCoding) ? 16 : (mCodecId == DecimalCoding) ? 10 : 8;

    QString string = QString::number(address, base);

    if (addressType == RelativeForwards) {
        string.prepend(QLatin1Char('+'));
    } else if (addressType == RelativeBackwards) {
        string.prepend(QLatin1Char('-'));
    }

    return string;
}

}

#include "moc_addressvalidator.cpp"
