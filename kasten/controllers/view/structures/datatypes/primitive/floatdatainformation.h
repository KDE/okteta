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
#ifndef FLOATDATAINFORMATION_H_
#define FLOATDATAINFORMATION_H_

#include "primitivedatainformation.h"

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
    return Type_Float;
}

inline float FloatDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    float result = value.toFloat(ok);
    //result != result if value is NaN so check for that case too
    if (double(result) != value.toDouble() && result == result)
        *ok = false;
    return result;
}

#endif /* FLOATDATAINFORMATION_H_ */
