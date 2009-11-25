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
#ifndef DATAINFORMATION_H_
#define DATAINFORMATION_H_

//Qt core
#include <QString>
#include <QList>
#include <QObject>
//Okteta
#include <size.h>
#include <address.h>
#include <abstractbytearraymodel.h>
//structview
#include "structviewpreferences.h"
//XML
#include <QtXml/QDomElement>
#include <QFileInfo>
//KDE
#include <KLocale>
#include <KGlobal>

#define DATAINFORMATION_CLONE(type) virtual inline DataInformation* clone() const {\
        return new type##DataInformation(*this); \
    }

//namespace Okteta
//{
//class AbstractByteArrayModel;
//}

typedef Kasten::StructViewPreferences::EnumByteOrder::type ByteOrder;
typedef Kasten::StructViewPreferences::EnumByteOrder ByteOrderEnumClass;

/** Interface that must be implemented by all datatypes */
class DataInformation: public QObject
{
Q_OBJECT
public:
//    Q_PROPERTY(bool valid READ isValid() STORED false)
    virtual bool isValid() const = 0;
protected:
    DataInformation(const DataInformation&);
public:
    virtual DataInformation* clone() const = 0;
    DataInformation(const QString& name, int index, DataInformation* parent = NULL);
    virtual ~DataInformation();
    /** true for unions and structs and arrays*/
    bool hasChildren() const
    {
        return false;
    }
    virtual unsigned int childCount() const
    {
        return 0;
    }
    /**
     *  column 0 is name
     *  column 1 is value
     */
    static const int COLUMN_COUNT = 3;

    virtual DataInformation* childAt(unsigned int) const
    {
        return NULL;
    }

    /** needs to be virtual for bitfields */
    virtual QString getSizeString() const;
    inline QString getName() const
    {
        return objectName();
    }
    virtual Okteta::Size positionRelativeToParent() const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const
    {
        Q_UNUSED(column)
        Q_UNUSED(fileLoaded);
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    int row()
    {
        //        if (mIndex == -1)
        //            kWarning("DataInformation::row(): mIndex == -1");
        return mIndex;
    }
    void setIndex(int newIndex)
    {
        mIndex = newIndex;
    }
    virtual bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, Okteta::Size remaining) = 0;
    /** get the necessary data (for the model) */
    virtual QVariant data(int column, int role) const =0;
    /** The size of this DataInformation type in bits (to allow bitfields in future) */
    virtual int getSize() const =0;
    virtual QString getTypeName() const =0;
    /** by default just returns an empty QString */
    virtual QString getValueString() const;
    /** reads the necessary data and returns the number of bytes read */
    virtual Okteta::Size
    readData(Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, Okteta::Size remaining) =0;
protected:
    static DataInformation* parseNode(QDomNode& n);
    virtual Okteta::Size offset(unsigned int index) const = 0;
Q_SIGNALS:
    void dataChanged();
protected Q_SLOTS:
    void onChildDataChanged()
    {
        emit dataChanged();
    }
protected:
    int mIndex;
};

#endif /* DATAINFORMATION_H_ */
