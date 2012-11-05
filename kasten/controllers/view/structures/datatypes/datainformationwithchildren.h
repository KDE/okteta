/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#include <QVector>

class DataInformationWithChildren: public DataInformation
{
protected:
    QVector<DataInformation*> mChildren;
    explicit DataInformationWithChildren(const DataInformationWithChildren& d);
public:
    explicit DataInformationWithChildren(const QString& name, const QVector<DataInformation*>& children
            = QVector<DataInformation*>(), DataInformation* parent = 0);
    virtual ~DataInformationWithChildren();

    virtual QVariant childData(int row, int column, int role) const;

    virtual BitCount32 size() const;
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);

    /** replaces child at @p index with @p newChild.
     * @param index the index of the child
     * @param newChild the new child (ownership is taken if replacing is successful,
     *  otherwise it must be deleted)
     * @return true if replacing was successful, false otherwise */
    virtual bool replaceChildAt(unsigned int index, DataInformation* newChild);
    virtual int indexOf(const DataInformation* const data) const;

    virtual DataInformation* childAt(unsigned int index) const;
    virtual bool canHaveChildren() const;
    virtual unsigned int childCount() const;
    virtual QString tooltipString() const;

    virtual QWidget* createEditWidget(QWidget* parent) const;
    /** get the needed data from the widget */
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual void resetValidationState();
    virtual void calculateValidationState();

    /** Takes ownership! */
    void appendChild(DataInformation* child, bool emitSignal = true);
    /** Takes ownership of all elements */
    void appendChildren(const QVector<DataInformation*>& newChildren, bool emitSignal = true);
    void setChildren(const QVector<DataInformation*>& newChildren);
    void setChildren(QScriptValue newChildren);

    static QVector<DataInformation*> cloneList(const QVector<DataInformation*>& other, DataInformation* parent);
protected:
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const;
};

inline unsigned int DataInformationWithChildren::childCount() const
{
    return mChildren.size();
}

inline bool DataInformationWithChildren::canHaveChildren() const
{
    return true;
}

#endif /* DATAINFORMATIONWITHCHILDREN_H_ */
