/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "doubledatainformation.hpp"

// tool
#include "structureviewpreferences.hpp"
// libdatatypeeditors
#include <float64editor.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>


QWidget* DoubleDataInformationMethods::staticCreateEditWidget(QWidget* parent)
{
    auto* ret = new Okteta::Float64Editor(parent);
    return ret;
}

QVariant DoubleDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    const auto* editor = qobject_cast<const Okteta::Float64Editor*> (w);
    Q_CHECK_PTR(editor);
    if (editor) {
        return editor->data().value;
    }
    return {};
}

void DoubleDataInformationMethods::staticSetWidgetData(double value, QWidget* w)
{
    auto* editor = qobject_cast<Okteta::Float64Editor*> (w);
    Q_CHECK_PTR(editor);
    if (editor) {
        editor->setData(value);
    }
}

QScriptValue DoubleDataInformationMethods::asScriptValue(double value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine)
    Q_UNUSED(handler)
    return {value};
}

QString DoubleDataInformationMethods::staticValueString(double value)
{
    return (Kasten::StructureViewPreferences::localeAwareFloatFormatting())
           ? QLocale().toString(value, 'g', Kasten::StructureViewPreferences::floatPrecision())
           : QString::number(value, 'g', Kasten::StructureViewPreferences::floatPrecision());
}
