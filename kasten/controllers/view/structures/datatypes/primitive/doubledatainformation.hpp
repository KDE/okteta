/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOUBLEDATAINFORMATION_HPP
#define KASTEN_DOUBLEDATAINFORMATION_HPP

#include "primitivedatainformation.hpp"

class DoubleDataInformationMethods
{
public:
    static QString staticValueString(double value);
    static PrimitiveDataType staticType();
    static QScriptValue asScriptValue(double value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static double fromVariant(const QVariant& value, bool* ok);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(double value, QWidget* w);
};

inline PrimitiveDataType DoubleDataInformationMethods::staticType()
{
    return PrimitiveDataType::Double;
}

inline double DoubleDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    double result = value.toDouble(ok);
    return result;
}

#endif /* KASTEN_DOUBLEDATAINFORMATION_HPP */
