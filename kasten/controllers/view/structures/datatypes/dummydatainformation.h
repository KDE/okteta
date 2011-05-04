/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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


#ifndef DUMMYDATAINFORMATION_H
#define DUMMYDATAINFORMATION_H

#include "datainformation.h"


class DummyDataInformation : public DataInformation
{
public:
    DummyDataInformation(DataInformationBase* parent);
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual bool setData(const QVariant& value, DataInformation* inf, Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset);
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            quint64 bitsRemaining, quint8* bitOffset);
    virtual int size() const;
    virtual void setWidgetData(QWidget* w) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QString typeName() const;
    virtual DataInformation* clone() const;
    virtual bool isDummy() const;
protected:
    virtual quint64 offset(unsigned int index) const;
};

inline bool DummyDataInformation::isDummy() const
{
    return true;
}

#endif // DUMMYDATAINFORMATION_H
