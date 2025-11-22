/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOUBLEDATAINFORMATION_HPP
#define KASTEN_DOUBLEDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"

class DoubleDataInformationMethods
{
public:
    [[nodiscard]]
    static QString staticValueString(double value);
    [[nodiscard]]
    static PrimitiveDataType staticType();
    [[nodiscard]]
    static QScriptValue asScriptValue(double value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    [[nodiscard]]
    static double fromVariant(const QVariant& value, bool* ok);
    [[nodiscard]]
    static QVariant staticToQVariant(double value);

    [[nodiscard]]
    static QWidget* staticCreateEditWidget(QWidget* parent);
    [[nodiscard]]
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(double value, QWidget* w);
};

inline PrimitiveDataType DoubleDataInformationMethods::staticType()
{
    return PrimitiveDataType::Double;
}

inline QVariant DoubleDataInformationMethods::staticToQVariant(double value)
{
    return {value};
}

inline double DoubleDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    double result = value.toDouble(ok);
    return result;
}

#endif /* KASTEN_DOUBLEDATAINFORMATION_HPP */
