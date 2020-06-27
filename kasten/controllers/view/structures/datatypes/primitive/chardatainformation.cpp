/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "chardatainformation.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QLocale>
#include <QScriptValue>
#include <QLineEdit>

#include "structureviewpreferences.h"

namespace {
QString charString(quint8 value)
{
    switch (value)
    {
    case '\0': return QStringLiteral("'\\0'");
    case '\a': return QStringLiteral("'\\a'");
    case '\b': return QStringLiteral("'\\b'");
    case '\f': return QStringLiteral("'\\f'");
    case '\n': return QStringLiteral("'\\n'");
    case '\r': return QStringLiteral("'\\r'");
    case '\t': return QStringLiteral("'\\t'");
    case '\v': return QStringLiteral("'\\v'");
    default: break;
    }
    QChar qchar = QChar(quint32(value));
    if (!qchar.isPrint()) {
        qchar = QChar::ReplacementCharacter;
    }
    return QString(QLatin1Char('\'') + qchar + QLatin1Char('\''));
}
}

QString CharDataInformationMethods::staticValueString(quint8 value)
{
    QString charStr = charString(value);
    if (Kasten::StructureViewPreferences::showCharNumericalValue()) {
        int base = Kasten::StructureViewPreferences::charDisplayBase();
        const QString num = (base == 10 && Kasten::StructureViewPreferences::localeAwareDecimalFormatting())
                            ? QLocale().toString(value)
                            : QString::number(value, base);
        charStr += QLatin1String(" (") + PrimitiveDataInformation::basePrefix(base)
                   + num + QLatin1Char(')');
    }
    return charStr;
}

QWidget* CharDataInformationMethods::staticCreateEditWidget(QWidget* parent)
{
    auto* editWidget = new QLineEdit(parent);
    editWidget->setClearButtonEnabled(true);
    return editWidget;
}

QVariant CharDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    // TODO fix this code!!
    const auto* edit = qobject_cast<const QLineEdit*> (w);
    if (edit) {
        QString text = edit->text();
        if (text.length() == 0) {
            return {};
        }
        if (text.length() == 1) {
            // TODO char codec
            return (unsigned char) text.at(0).toLatin1();
        }
        if (text.at(0) == QLatin1Char('\\')) {
            // escape sequence
            if (text.at(1) == QLatin1Char('x')) {
                // hex escape:
                bool okay;
                const QStringRef valStr = text.midRef(2, 2); // only 2 chars
                quint8 val = valStr.toInt(&okay, 16);
                if (okay) {
                    return val;
                }
                return {};
            }
            if (text.at(1) == QLatin1Char('n')) {
                return (quint8) '\n'; // newline
            }
            if (text.at(1) == QLatin1Char('t')) {
                return (quint8) '\t'; // tab
            }
            if (text.at(1) == QLatin1Char('r')) {
                return (quint8) '\r'; // cr
            }
            // octal escape:
            bool okay;
            const QStringRef valStr = text.midRef(1, 3); // only 2 chars
            quint8 val = valStr.toInt(&okay, 8);
            if (okay) {
                return val;
            }
            return {};
        }
    }
    return {};
}

void CharDataInformationMethods::staticSetWidgetData(quint8 value, QWidget* w)
{
    auto* edit = qobject_cast<QLineEdit*> (w);
    if (edit) {
        QChar qchar(value, 0);
        if (!qchar.isPrint()) {
            qchar = QChar(QChar::ReplacementCharacter);
        }
        edit->setText(qchar);
    }
}

QScriptValue CharDataInformationMethods::asScriptValue(quint8 value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(QString(value > 127 ? QChar::ReplacementCharacter : QChar(value, 0)));
}
