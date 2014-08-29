/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011  Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "floatdatainformation.h"

#include <limits>
// KF5
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QDoubleSpinBox>
#include <QLocale>

#include "structviewpreferences.h"

QWidget* FloatDataInformationMethods::staticCreateEditWidget(QWidget* parent)
{
    QDoubleSpinBox* ret = new QDoubleSpinBox(parent);
    ret->setMinimum(std::numeric_limits<float>::min());
    ret->setMaximum(std::numeric_limits<float>::max());
    return ret;
}

QVariant FloatDataInformationMethods::staticDataFromWidget(const QWidget* w)
{
    const QDoubleSpinBox* spin = qobject_cast<const QDoubleSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        return ((float) spin->value());
    return QVariant();
}

void FloatDataInformationMethods::staticSetWidgetData(float value, QWidget* w)
{
    QDoubleSpinBox* spin = qobject_cast<QDoubleSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

QScriptValue FloatDataInformationMethods::asScriptValue(float value, QScriptEngine* engine, ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(value);
}

QString FloatDataInformationMethods::staticValueString(float value)
{
    return (Kasten::StructViewPreferences::localeAwareFloatFormatting())
        ? QLocale().toString(value, 'g', Kasten::StructViewPreferences::floatPrecision())
        : QString::number(value, 'g', Kasten::StructViewPreferences::floatPrecision());
}
