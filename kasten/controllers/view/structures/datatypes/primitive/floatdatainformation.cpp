/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "floatdatainformation.hpp"

#include <limits>
// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QDoubleSpinBox>
#include <QLocale>

#include "structureviewpreferences.h"

QWidget* FloatDataInformationMethods::staticCreateEditWidget(QWidget* parent)
{
    auto* ret = new QDoubleSpinBox(parent);
    ret->setMinimum(std::numeric_limits<float>::min());
    ret->setMaximum(std::numeric_limits<float>::max());
    return ret;
}

QVariant FloatDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    const auto* spin = qobject_cast<const QDoubleSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin) {
        return ((float) spin->value());
    }
    return {};
}

void FloatDataInformationMethods::staticSetWidgetData(float value, QWidget* w)
{
    auto* spin = qobject_cast<QDoubleSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin) {
        spin->setValue(value);
    }
}

QScriptValue FloatDataInformationMethods::asScriptValue(float value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return {value};
}

QString FloatDataInformationMethods::staticValueString(float value)
{
    return (Kasten::StructureViewPreferences::localeAwareFloatFormatting())
           ? QLocale().toString(value, 'g', Kasten::StructureViewPreferences::floatPrecision())
           : QString::number(value, 'g', Kasten::StructureViewPreferences::floatPrecision());
}
