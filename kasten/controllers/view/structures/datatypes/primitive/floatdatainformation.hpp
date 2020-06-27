/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FLOATDATAINFORMATION_HPP
#define KASTEN_FLOATDATAINFORMATION_HPP

#include "primitivedatainformation.hpp"

class FloatDataInformationMethods
{
public:
    static PrimitiveDataType staticType();
    static QString staticValueString(float value);
    static QScriptValue asScriptValue(float value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static float fromVariant(const QVariant& value, bool* ok);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(float value, QWidget* w);
};

inline PrimitiveDataType FloatDataInformationMethods::staticType()
{
    return PrimitiveDataType::Float;
}

inline float FloatDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    float result = value.toFloat(ok);
    // result != result if value is NaN so check for that case too
    if (double(result) != value.toDouble() && result == result) {
        *ok = false;
    }
    return result;
}

#endif /* KASTEN_FLOATDATAINFORMATION_HPP */
