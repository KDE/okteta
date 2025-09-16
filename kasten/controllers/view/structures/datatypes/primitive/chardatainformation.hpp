/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARDATAINFORMATION_HPP
#define KASTEN_CHARDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"

class CharDataInformationMethods
{
public:
    static QString staticValueString(quint8 value);
    static quint8 fromVariant(const QVariant& value, bool* ok);
    static PrimitiveDataType staticType();
    static QScriptValue asScriptValue(quint8 value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static QVariant staticToQVariant(quint8 value);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(quint8 value, QWidget* w);
};

inline PrimitiveDataType CharDataInformationMethods::staticType()
{
    return PrimitiveDataType::Char;
}

inline QVariant CharDataInformationMethods::staticToQVariant(quint8 value)
{
    return QVariant(value);
}

inline quint8 CharDataInformationMethods::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    quint32 tmp = value.toUInt(ok);
    auto result = static_cast<quint8>(tmp);
    if (tmp != quint32(result)) {
        *ok = false; // out of bounds
        return 0;
    }
    return result;
}

#endif /* KASTEN_CHARDATAINFORMATION_HPP */
