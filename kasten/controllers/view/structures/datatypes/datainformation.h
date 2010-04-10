/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
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
#ifndef DATAINFORMATION_H_
#define DATAINFORMATION_H_

//Qt core
#include <QString>
#include <QList>
#include <QObject>
#include <QWidget>

//Okteta
#include <size.h>
#include <address.h>

#include "structviewpreferences.h"
#include "additionaldata.h"

//KDE
#include <KLocale>
#include <KGlobal>

//Qt
#include <QtScript/QScriptValue>

#define DATAINFORMATION_CLONE(type) virtual inline DataInformation* clone() const {\
        return new type##DataInformation(*this); \
    }

namespace Okteta
{
class AbstractByteArrayModel;
}

class TopLevelDataInformation;

typedef Kasten::StructViewPreferences::EnumByteOrder::type ByteOrder;
typedef Kasten::StructViewPreferences::EnumByteOrder ByteOrderEnumClass;

/** Interface that must be implemented by all datatypes */
class DataInformation: public QObject
{
Q_OBJECT
public:
    Q_PROPERTY(bool hasBeenValidated READ hasBeenValidated() WRITE setHasBeenValidated)
    Q_PROPERTY(bool validationSuccessful READ validationSuccessful() WRITE setValidationSuccessful)
    Q_PROPERTY(bool wasAbleToRead READ wasAbleToRead())
    Q_PROPERTY(AdditionalData* additionalData READ additionalData())

    virtual bool wasAbleToRead() const = 0;
protected:
    DataInformation(const DataInformation&);
public:
    virtual DataInformation* clone() const = 0;
    DataInformation(const QString& name, int index, DataInformation* parent = NULL);
    virtual ~DataInformation();
    /**
     *  column 0 is name
     *  column 1 is value
     */
    static const int COLUMN_COUNT = 3;

    //methods for children:
    /** true for unions and structs and arrays*/
    virtual bool hasChildren() const;
    virtual unsigned int childCount() const;
    virtual DataInformation* childAt(unsigned int) const;
    virtual void setIndex(int newIndex);
    virtual quint64 positionRelativeToParent() const;

    //for the model:
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    int row() const;
    /** get the necessary data (for the model) */
    virtual QVariant data(int column, int role) const = 0;
    /** The size of this DataInformation type in bits (to allow bitfields in future) */
    virtual QString typeName() const = 0;
    /** by default just returns an empty QString */
    virtual QString valueString() const;
    virtual QString name() const;
    /** needs to be virtual for bitfields */
    virtual QString sizeString() const;

    //delegate functions:
    /** create a QWidget for the QItemDelegate */
    virtual QWidget* createEditWidget(QWidget* parent) const = 0;
    /** get the needed data from the widget */
    virtual QVariant dataFromWidget(const QWidget* w) const = 0;
    /** initialize the delegate widget with the correct data */
    virtual void setWidgetData(QWidget* w) const = 0;

    //reading and writing
    virtual int size() const = 0;
    /** Reads the necessary data from @p input and returns the number of bytes read
     *
     * @param input the byte array to read from
     * @param byteOrder the byte order used to read the data (big/little endian)
     * @param address the starting offset to read from
     * @param bitsRemaining the number of bits remaining in @p out
     * @param bitOffset the bits that have already been read from the current byte
     *        (should be modified in this method)
     *
     * @return the number of bits read or @c -1 if none were read
     */
    virtual qint64
    readData(Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset) =0;
    /** Writes the current data contained in this object to out.
     *
     *  If @code @p inf != this @endcode this method must do nothing and return false.
     *
     *  @param value a @link QVariant object holding the new data to write
     *  @param inf the object that should currently write the data
     *  @param out the byte array the value is read from
     *  @param byteOrder the byteOrder used for reading values
     *  @param address the address in @p out
     *  @param bitsRemaining number of bits remaining in @p input
     *  @param bitOffset the bit to start at in the first byte
     *
     *  @return @c true on success, @c false otherwise
     */
    virtual bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset) = 0;

    virtual bool isDynamicArray() const;
    // this method cannot be const since the final return value is a this-pointer
    virtual TopLevelDataInformation* topLevelDataInformation();

    void setAdditionalData(AdditionalData* data);
    AdditionalData* additionalData() const;Q_INVOKABLE
    void setValidationError(QString errorMessage);Q_INVOKABLE
    void validated(bool success = true);

    bool validationSuccessful() const;
    void setValidationSuccessful(bool validationSuccessful);
    bool hasBeenValidated() const;
    void setHasBeenValidated(bool hasBeen);

protected:
    /**
     *  the offset of child number @p index compared to the beginning of the structure
     *  in bits.
     *  @param index the index of the child
     *  @return 0 unless this DataInformation has children
     */
    virtual quint64 offset(unsigned int index) const = 0;
Q_SIGNALS:
    void dataChanged();
    void childCountChange(int oldCount, int newCount);
protected:
    int mIndex;
    AdditionalData* mAdditionalData;
    bool mValidationSuccessful :1;
    bool mHasBeenValidated :1;
};

//inline functions
inline int DataInformation::row() const
{
    //if (mIndex == -1)
    //    kWarning("DataInformation::row(): mIndex == -1");
    return mIndex;
}

inline Qt::ItemFlags DataInformation::flags(int column, bool fileLoaded) const
{
    Q_UNUSED(column)
    Q_UNUSED(fileLoaded);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

inline QString DataInformation::name() const
{
    return objectName();
}

inline DataInformation* DataInformation::childAt(unsigned int) const
{
    return NULL;
}

inline bool DataInformation::hasChildren() const
{
    return false;
}

inline unsigned int DataInformation::childCount() const
{
    return 0;
}

inline bool DataInformation::isDynamicArray() const
{
    return false;
}

inline AdditionalData* DataInformation::additionalData() const
{
    return mAdditionalData;
}

inline bool DataInformation::validationSuccessful() const
{
    return mValidationSuccessful;
}
inline void DataInformation::setValidationSuccessful(bool successful)
{
    mValidationSuccessful = successful;
}

inline bool DataInformation::hasBeenValidated() const
{
    return mHasBeenValidated;
}
inline void DataInformation::setHasBeenValidated(bool hasBeen)
{
    mHasBeenValidated = hasBeen;
}
#endif /* DATAINFORMATION_H_ */
