/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef SIGNEDBITFIELDDATAINFORMATION_H_
#define SIGNEDBITFIELDDATAINFORMATION_H_

#include "abstractbitfielddatainformation.h"
#include "../poddecoder/typeeditors/sintspinbox.h"

class SignedBitfieldDataInformation: public AbstractBitfieldDataInformation
{
public:
    SignedBitfieldDataInformation(QString name, uint width, DataInformation* parent = 0) :
        AbstractBitfieldDataInformation(name, width, parent)
    {
    }
    virtual ~SignedBitfieldDataInformation()
    {
    }
protected:
    SignedBitfieldDataInformation(const SignedBitfieldDataInformation& d) :
        AbstractBitfieldDataInformation(d)
    {
    }
private:
public:
    DATAINFORMATION_CLONE(SignedBitfield)

    QString typeName() const;
    virtual QString valueString() const;
    virtual QScriptValue valueAsQScriptValue() const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    int displayBase() const;
};

inline QString SignedBitfieldDataInformation::typeName() const
{
    return i18nc("Data type", "signed bitfield");
}
#endif /* SIGNEDBITFIELDDATAINFORMATION_H_ */
