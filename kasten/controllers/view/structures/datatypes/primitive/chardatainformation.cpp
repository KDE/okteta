/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "chardatainformation.hpp"

// lib
#include "structureviewpreferences.hpp"
// libdatatypeeditors
#include <char8editor.hpp>
// Okteta core
#include <Okteta/CharCodec>
// KF
#include <KLocalizedString>
// Qt
#include <QLocale>
#include <QScriptValue>


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
    QChar qchar = (value > 127) ? QChar::ReplacementCharacter : QChar(value, 0);
    if (!qchar.isPrint()) {
        qchar = QChar::ReplacementCharacter;
    }
    return QString(QLatin1Char('\'') + qchar + QLatin1Char('\''));
}
}

class CodecOwner : public QObject
{
public:
    CodecOwner(const Okteta::CharCodec* charCodec, QObject* parent);

private:
    std::unique_ptr<const Okteta::CharCodec> m_charCodec;
};

CodecOwner::CodecOwner(const Okteta::CharCodec* charCodec, QObject* parent)
    : QObject(parent)
    , m_charCodec(charCodec)
{
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
    const Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec(QStringLiteral("US-ASCII"));
    auto* editWidget = new Okteta::Char8Editor(charCodec, parent);
    new CodecOwner(charCodec, editWidget);
    return editWidget;
}

QVariant CharDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    // TODO fix this code!!
    const auto* edit = qobject_cast<const Okteta::Char8Editor*> (w);
    if (edit) {
        const std::optional<quint8> byte = edit->byte();
        if (byte) {
            return byte.value();
        }
    }
    return {};
}

void CharDataInformationMethods::staticSetWidgetData(quint8 value, QWidget* w)
{
    auto* edit = qobject_cast<Okteta::Char8Editor*>(w);
    if (edit) {
        edit->setByte(value);
    }
}

QScriptValue CharDataInformationMethods::asScriptValue(quint8 value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(QString(value > 127 ? QChar::ReplacementCharacter : QChar(value, 0)));
}
