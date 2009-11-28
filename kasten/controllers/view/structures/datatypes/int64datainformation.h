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
#ifndef INT64DATAINFORMATION_H_
#define INT64DATAINFORMATION_H_

#include "signedprimitivedatainformation.h"
#include "../poddecoder/typeeditors/sint64editor.h"

class Int64DataInformation: public SignedPrimitiveDataInformation
{
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTURUCTORS(Int64,SignedPrimitive)

public:
    inline int getSize() const
    {
        return 64;
    }
    inline QString getTypeName() const
    {
        return i18nc("Data type", "long");
    }
    DATAINFORMATION_CLONE(Int64)
    virtual QString getValueString() const;

    virtual void setWidgetData(QWidget* w) const;

    inline QWidget* createEditWidget(QWidget* parent) const
    {
        SInt64Editor* ret = new SInt64Editor(parent);
        ret->setBase(displayBase());
        return ret;
    }
};

#endif /* INT64DATAINFORMATION_H_ */
