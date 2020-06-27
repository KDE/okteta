/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "doubledatainformation.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>
#include <QDoubleSpinBox>

#include "structureviewpreferences.h"

QWidget* DoubleDataInformationMethods::staticCreateEditWidget(QWidget* parent)
{
    auto* ret = new QDoubleSpinBox(parent);
    return ret;
}

QVariant DoubleDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    const auto* spin = qobject_cast<const QDoubleSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin) {
        return spin->value();
    }
    return {};
}

void DoubleDataInformationMethods::staticSetWidgetData(double value, QWidget* w)
{
    auto* spin = qobject_cast<QDoubleSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin) {
        spin->setValue(value);
    }
}

QScriptValue DoubleDataInformationMethods::asScriptValue(double value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return {value};
}

QString DoubleDataInformationMethods::staticValueString(double value)
{
    return (Kasten::StructureViewPreferences::localeAwareFloatFormatting())
           ? QLocale().toString(value, 'g', Kasten::StructureViewPreferences::floatPrecision())
           : QString::number(value, 'g', Kasten::StructureViewPreferences::floatPrecision());
}
