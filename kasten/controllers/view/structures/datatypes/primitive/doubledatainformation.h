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
#ifndef DOUBLEDATAINFORMATION_H_
#define DOUBLEDATAINFORMATION_H_

#include "primitivedatainformation.h"

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
    return Type_Double;
}

inline double DoubleDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    double result = value.toDouble(ok);
    return result;
}

#endif /* DOUBLEDATAINFORMATION_H_ */
