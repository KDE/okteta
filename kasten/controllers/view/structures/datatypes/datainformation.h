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
#ifndef DATAINFORMATION_H_
#define DATAINFORMATION_H_

//Qt core
#include <QString>
#include <QList>

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

#include "datainformationbase.h"

#define DATAINFORMATION_CLONE(type) virtual inline type##DataInformation* clone() const {\
        return new type##DataInformation(*this); \
    }

namespace Okteta
{
class AbstractByteArrayModel;
}

class TopLevelDataInformation;
class ScriptHandlerInfo;
class QScriptContext;
class QScriptEngine;

/** Interface that must be implemented by all datatypes */
class DataInformation: public DataInformationBase
{
protected:
    explicit DataInformation(const DataInformation&);
public:
    virtual DataInformation* clone() const = 0;
    explicit DataInformation(const QString& name, DataInformationBase* parent = NULL);
    virtual ~DataInformation();

    enum Columns
    {
        ColumnName = 0, ColumnType, ColumnValue, COLUMN_COUNT
    };
    enum DataInformationEndianess {
        EndianessFromSettings = 0, EndianessInherit, EndianessLittle, EndiannessBig
    };

    //methods for children:
    virtual bool hasChildren() const;
    /** true for unions and structs and arrays*/
    virtual bool canHaveChildren() const;
    virtual unsigned int childCount() const;
    virtual DataInformation* childAt(unsigned int) const;
    /** @brief Looks for a child of this object with given name
     *  @param name The name of the child to find
     *  @return the child with given @p name or @c NULL if none found with that name
     */
    virtual DataInformation* child(QString name) const;
    virtual BitCount32 positionRelativeToRoot(int row = -1) const;

    //for the model:
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    int row() const;
    /** get the necessary data (for the model) */
    virtual QVariant data(int column, int role) const;
    /** the data of child at index @p row. Useful for arrays, or DataInformations with fake children*/
    virtual QVariant childData(int row, int column, int role) const;
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const;
    virtual BitCount32 childSize(int index) const = 0;


    /** The size of this DataInformation type in bits (to allow bitfields in future) */
    virtual QString typeName() const = 0;
    /** by default just returns an empty QString */
    virtual QString valueString() const;
    virtual QString name() const;
    virtual QString tooltipString() const;
    /** needs to be virtual for bitfields */
    virtual QString sizeString() const;

    //delegate functions:
    /** create a QWidget for the QItemDelegate */
    virtual QWidget* createEditWidget(QWidget* parent) const = 0;
    /** get the needed data from the widget */
    virtual QVariant dataFromWidget(const QWidget* w) const = 0;
    /** initialize the delegate widget with the correct data */
    virtual void setWidgetData(QWidget* w) const = 0;

    /** create a QWidget for the QItemDelegate */
    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const;
    /** get the needed data from the widget */
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const;
    /** initialize the delegate widget with the correct data */
    virtual void setChildWidgetData(uint index, QWidget* w) const;

    //reading and writing
    /** the size in bits of this element */
    virtual BitCount32 size() const = 0;

    /** Reads the necessary data from @p input and returns the number of bytes read
     *
     * @param input the byte array to read from
     * @param address the starting offset to read from
     * @param bitsRemaining the number of bits remaining in @p out
     * @param bitOffset the bits that have already been read from the current byte
     *        (should be modified in this method)
     *
     * @return the number of bits read or @c -1 if none were read
     */
    virtual qint64 readData(Okteta::AbstractByteArrayModel *input, Okteta::Address address,
             BitCount64 bitsRemaining, quint8* bitOffset) = 0;
    /** sets mWasAbleToRead to false for all children and this object.
     *  Gets called once before the reading of the whole structure starts. */
    void beginRead();
    /** Writes the current data contained in this object to out.
     *
     *  @param value a @link QVariant object holding the new data to write
     *  @param out the byte array the value is written to
     *  @param address the address in @p out
     *  @param bitsRemaining number of bits remaining in @p out
     *  @param bitOffset the bit to start at in the first byte
     *
     *  @return @c true on success, @c false otherwise
     */
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) = 0;
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) = 0;

    virtual bool isTopLevel() const;
    TopLevelDataInformation* topLevelDataInformation() const;
    DataInformation* mainStructure();

    void setAdditionalData(AdditionalData* data);
    AdditionalData* additionalData() const;
    QString validationError() const;
    void setValidationError(QString errorMessage);

    bool validationSuccessful() const;
    void setValidationSuccessful(bool validationSuccessful = true);
    bool hasBeenValidated() const;
    void setHasBeenValidated(bool hasBeen);
    DataInformationEndianess byteOrder() const;
    ByteOrder effectiveByteOrder() const;
    void setByteOrder(DataInformationEndianess newEndianess);

    virtual void resetValidationState(); //virtual for datainformationwithchildren
    bool wasAbleToRead() const;
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) = 0;
    void setParent(DataInformationBase* newParent);
    DataInformationBase* parent() const;
    QPair<DataInformation*, QString> findChildForDynamicArrayLength(const QString& name, uint upTo) const;
protected:
    /**
     *  the offset of child number @p index compared to the beginning of the structure in bits.
     *  @param index the index of the child
     *  @return 0 unless this DataInformation has children
     */
    virtual BitCount32 offset(unsigned int index) const = 0;
    /**
     * Find the index of a DataInformation in this object, needed to calculate the row
     */
    virtual int indexOf(const DataInformation* const data) const; //TODO make this pure virtual
protected:
    bool mValidationSuccessful :1;
    bool mHasBeenValidated :1;
    bool mWasAbleToRead :1;
    DataInformationEndianess mByteOrder : 2;
    AdditionalData* mAdditionalData;
    DataInformationBase* mParent;
    QString mName;
};

Q_DECLARE_METATYPE(DataInformation*)
Q_DECLARE_METATYPE(const DataInformation*)

inline Qt::ItemFlags DataInformation::flags(int column, bool fileLoaded) const
{
    Q_UNUSED(column)
    Q_UNUSED(fileLoaded);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

inline QString DataInformation::name() const
{
    return mName;
}

inline DataInformation* DataInformation::childAt(unsigned int) const
{
    return NULL;
}

inline bool DataInformation::hasChildren() const
{
    return false;
}

inline bool DataInformation::canHaveChildren() const
{
    return false;
}

inline unsigned int DataInformation::childCount() const
{
    return 0;
}

inline AdditionalData* DataInformation::additionalData() const
{
    return mAdditionalData;
}

inline bool DataInformation::wasAbleToRead() const
{
    return mWasAbleToRead;
}

inline ByteOrder DataInformation::effectiveByteOrder() const
{
    switch (mByteOrder)
    {
        case EndiannessBig:
            return ByteOrderEnumClass::BigEndian;
        case EndianessLittle:
            return ByteOrderEnumClass::LittleEndian;
        case EndianessFromSettings:
            return Kasten2::StructViewPreferences::byteOrder();
        case EndianessInherit:
            return (mParent && !mParent->isTopLevel()) ?
                mParent->asDataInformation()->effectiveByteOrder()
                : Kasten2::StructViewPreferences::byteOrder();
    }

    // here must be a return... I guess this is correct
    return Kasten2::StructViewPreferences::byteOrder();
}

inline DataInformation::DataInformationEndianess DataInformation::byteOrder() const
{
    return mByteOrder;
}


inline void DataInformation::setByteOrder(DataInformation::DataInformationEndianess newByteOrder)
{
    mByteOrder = newByteOrder;
}

inline bool DataInformation::isTopLevel() const
{
    return false;
}

inline void DataInformation::setParent(DataInformationBase* newParent)
{
    Q_CHECK_PTR(newParent);
    mParent = newParent;
}

inline DataInformationBase* DataInformation::parent() const
{
    return mParent;
}

#endif /* DATAINFORMATION_H_ */
