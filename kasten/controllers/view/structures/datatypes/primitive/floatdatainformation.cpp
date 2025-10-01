/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "floatdatainformation.hpp"

// tool
#include <structureviewpreferences.hpp>
// libdatatypeeditors
#include <float32editor.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>


QWidget* FloatDataInformationMethods::staticCreateEditWidget(QWidget* parent)
{
    auto* const ret = new Okteta::Float32Editor(parent);
    return ret;
}

QVariant FloatDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    const auto* const editor = qobject_cast<const Okteta::Float32Editor*>(w);
    Q_CHECK_PTR(editor);
    if (editor) {
        return editor->data().value;
    }
    return {};
}

void FloatDataInformationMethods::staticSetWidgetData(float value, QWidget* w)
{
    auto* const editor = qobject_cast<Okteta::Float32Editor*>(w);
    Q_CHECK_PTR(editor);
    if (editor) {
        editor->setData(value);
    }
}

QScriptValue FloatDataInformationMethods::asScriptValue(float value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine)
    Q_UNUSED(handler)
    return {value};
}

QString FloatDataInformationMethods::staticValueString(float value)
{
    return (Kasten::StructureViewPreferences::localeAwareFloatFormatting())
           ? QLocale().toString(value, 'g', Kasten::StructureViewPreferences::floatPrecision())
           : QString::number(value, 'g', Kasten::StructureViewPreferences::floatPrecision());
}
