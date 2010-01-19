/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef ENUMDATAINFORMATION_H_
#define ENUMDATAINFORMATION_H_
#include "primitivedatainformation.h"
#include "enumdefinition.h"
class EnumDataInformation: public PrimitiveDataInformation
{
Q_OBJECT
public:
    EnumDataInformation(QString name, PrimitiveDataInformation* type,
            EnumDefinition::Ptr enumDef, int index = -1, DataInformation* parent =
                    NULL);
    EnumDataInformation(const EnumDataInformation& e);
    DATAINFORMATION_CLONE(Enum)
    virtual ~EnumDataInformation();
    QString getValueString() const;
    QString getTypeString() const;
    QString getTypeName() const;
    QVariant primitiveValue() const;
    inline int getSize() const
    {
        return mValue->getSize();
    }
    inline int displayBase() const
    {
        return mValue->displayBase();
    }
    Okteta::Size readData(Okteta::AbstractByteArrayModel* input,
            ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining,
            quint8* bitOffset);
    bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
            Okteta::Address address, Okteta::Size remaining, quint8* bitOffset);

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;

private:
    EnumDefinition::Ptr mEnum;
    PrimitiveDataInformation* mValue; //to allow different enum sizes
};

#endif /* ENUMDATAINFORMATION_H_ */
