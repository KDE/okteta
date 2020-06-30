/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARDATAINFORMATION_HPP
#define KASTEN_CHARDATAINFORMATION_HPP

#include "primitivedatainformation.hpp"

class CharDataInformationMethods
{
public:
    static QString staticValueString(quint8 value);
    static quint8 fromVariant(const QVariant& value, bool* ok);
    static PrimitiveDataType staticType();
    static QScriptValue asScriptValue(quint8 value, QScriptEngine* engine, ScriptHandlerInfo* handler);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(quint8 value, QWidget* w);
};

inline PrimitiveDataType CharDataInformationMethods::staticType()
{
    return PrimitiveDataType::Char;
}

inline quint8 CharDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    quint32 tmp = value.toUInt(ok);
    quint8 result = quint8(tmp);
    if (tmp != quint32(result)) {
        *ok = false; // out of bounds
        return 0;
    }
    return result;
}

#endif /* KASTEN_CHARDATAINFORMATION_HPP */
