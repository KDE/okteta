/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FLOATDATAINFORMATION_HPP
#define KASTEN_FLOATDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"

class FloatDataInformationMethods
{
public:
    [[nodiscard]]
    static PrimitiveDataType staticType();
    [[nodiscard]]
    static QString staticValueString(float value);
    [[nodiscard]]
    static QScriptValue asScriptValue(float value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    [[nodiscard]]
    static float fromVariant(const QVariant& value, bool* ok);
    [[nodiscard]]
    static QVariant staticToQVariant(float value);

    [[nodiscard]]
    static QWidget* staticCreateEditWidget(QWidget* parent);
    [[nodiscard]]
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(float value, QWidget* w);
};

inline PrimitiveDataType FloatDataInformationMethods::staticType()
{
    return PrimitiveDataType::Float;
}

inline QVariant FloatDataInformationMethods::staticToQVariant(float value)
{
    return {value};
}

inline float FloatDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    float result = value.toFloat(ok);
    // result != result if value is NaN so check for that case too
    if (double(result) != value.toDouble() && result == result) {
        *ok = false;
    }
    return result;
}

#endif /* KASTEN_FLOATDATAINFORMATION_HPP */
