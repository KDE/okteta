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
#ifndef CHARDATAINFORMATION_H_
#define CHARDATAINFORMATION_H_

#include "primitivedatainformation.h"

class CharDataInformation: public PrimitiveDataInformation
{
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(Char,Primitive)
public:
    DATAINFORMATION_CLONE(Char)

    static int displayBase();
    virtual BitCount32 size() const;
    virtual QString typeName() const;
    virtual QString valueString() const;
    static QString valueString(quint8 value);
    static quint8 fromVariant(const QVariant& value);
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;
    virtual PrimitiveDataType type() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);
    static QScriptValue asScriptValue(quint8 value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    virtual QScriptValue valueAsQScriptValue() const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(quint8 value, QWidget* w);
private:
    quint8 mValue;
};

inline PrimitiveDataType CharDataInformation::type() const
{
    return Type_Char;
}

inline BitCount32 CharDataInformation::size() const
{
    return sizeof(quint8) * 8;
}

inline QString CharDataInformation::typeName() const
{
    return i18nc("Data type", "char");
}

inline quint8 CharDataInformation::fromVariant(const QVariant& value)
{
    return quint8(value.toUInt());
}

#endif /* CHARDATAINFORMATION_H_ */
