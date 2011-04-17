/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef DATAINFORMATIONWITHCHILDREN_H_
#define DATAINFORMATIONWITHCHILDREN_H_

#include "datainformation.h"
#include <QtScript/QScriptValue>

class DataInformationWithChildren: public DataInformation
{
Q_OBJECT
    Q_PROPERTY(int childCount READ childCount)
    Q_PROPERTY(QScriptValue children READ childrenAsScriptValue)
protected:
    QList<DataInformation*> mChildren;
    explicit DataInformationWithChildren(const DataInformationWithChildren& d);
    void appendChild(DataInformation* child); //not part of public API (no adding to array)
    virtual int indexOf(const DataInformation* const data) const;
public:

    QList<DataInformation*> children() const;
    explicit DataInformationWithChildren(QString& name, DataInformation* parent = NULL);
    virtual ~DataInformationWithChildren();

    virtual QVariant data(int, int) const;
    virtual QVariant childData(int row, int column, int role) const;

    virtual int size() const;
    /** this is valid for structs and arrays, union has an own implementation */
    virtual bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, Okteta::Address address,
            quint64 bitsRemaining, quint8* bitOffset);
    /** this is valid for structs and arrays, union has an own implementation */
    virtual qint64 readData(Okteta::AbstractByteArrayModel *input,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset);

    virtual DataInformation* childAt(unsigned int index) const;
    virtual bool hasChildren() const;
    virtual unsigned int childCount() const;
    virtual QList<const DataInformation*> findChildrenWithName(const QString& name,
            const DataInformation* const upTo) const;
    quint64 offset(unsigned int index) const;

    virtual QWidget* createEditWidget(QWidget* parent) const;
    /** get the needed data from the widget */
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual void resetValidationState();
    virtual void calculateValidationState(); //only in DataInformationWithChildren
    //for QtScript
    Q_INVOKABLE void setChildren(QScriptValue children);
    /** returns an object that holds the children, accessable as obj["childName"] */
    virtual QScriptValue childrenAsScriptValue() const;
    /** alternate way to access children: obj.child("childName") */
    Q_INVOKABLE QScriptValue child(QString name) const;
    Q_INVOKABLE QScriptValue at(uint index) const;
};

inline QList<DataInformation*> DataInformationWithChildren::children() const
{
    return mChildren;
}
inline bool DataInformationWithChildren::hasChildren() const
{
    return childCount() != 0;
}

inline unsigned int DataInformationWithChildren::childCount() const
{
    return mChildren.size();
}

#endif /* DATAINFORMATIONWITHCHILDREN_H_ */
