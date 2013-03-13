/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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
#include <QPair>
#include <QScopedPointer>
#include <QMetaType>

//Okteta
#include <size.h>
#include <address.h>

#include "datainformationbase.h"
#include "../script/scriptlogger.h"
#include "primitivedatatype.h"

/** Implement the clone() method and add the copy constructor declaration
 * After this macro visibility will be set to protected */
#define DATAINFORMATION_CLONE_DECL(type, supertype) public: \
        virtual inline type* clone() const { return new type(*this); } \
    protected: \
        type(const type& d)

/** Implements the clone() method and add the start of the copy constructor (up to delegating).
 *  After this macro visibility will be set to protected */
#define DATAINFORMATION_CLONE(type, supertype) DATAINFORMATION_CLONE_DECL(type, supertype) : supertype(d)

namespace Okteta
{
class AbstractByteArrayModel;
}

class QScriptContext;
class QScriptEngine;
class QScriptValue;
class QScriptClass;
class QVariant;
class QWidget;
class ScriptLogger;
class AdditionalData;
class TopLevelDataInformation;
class ScriptHandlerInfo;

/** Interface that must be implemented by all datatypes */
class DataInformation : public DataInformationBase
{
    friend class ScriptHandler; //so mHasBeenUpdated/hasBeenValidated can be set
    friend class PrimitiveDataInformationWrapper; //to set mHasBeenUpdated
    friend class PointerDataInformation; //to set mWasAbleToRead on pointer target
    friend class DefaultScriptClass; //to set mValidationSucessful and validationError
    template<PrimitiveDataTypeEnum type> friend class PrimitiveArrayData; //to set mWasAbleToRead (when returning a script value)
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
    enum DataInformationEndianess
    {
        EndianessFromSettings = 0, EndianessInherit, EndianessLittle, EndiannessBig
    };

    //methods for children:
    /** true for unions and structs and arrays*/
    virtual bool canHaveChildren() const = 0;
    virtual unsigned int childCount() const = 0;
    virtual DataInformation* childAt(unsigned int) const = 0;
    /** @brief Looks for a child of this object with given name
     *  @param name The name of the child to find
     *  @return the child with given @p name or @c NULL if none found with that name
     */
    virtual DataInformation* child(const QString& name) const;
    /**
     * @param start the starting address of the whole structure
     * @return the position in the file where this element is located
     */
    virtual BitCount64 positionInFile(Okteta::Address start) const;
    /**
     * @param child the direct child we want to find the address for
     * @param start the start of the root element
     * @return the address of @p child in the file
     */
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const = 0;

    //for the model:
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    int row() const;
    /** get the necessary data (for the model) */
    virtual QVariant data(int column, int role) const;

    virtual QString typeName() const = 0;
    /** by default just returns an empty QString */
    virtual QString valueString() const;
    QString name() const;
    void setName(const QString& newName);
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

    virtual bool isTopLevel() const;
    TopLevelDataInformation* topLevelDataInformation() const;
    DataInformation* mainStructure();

    QScriptValue updateFunc() const;
    void setUpdateFunc(const QScriptValue& func);
    QScriptValue validationFunc() const;
    void setValidationFunc(const QScriptValue& func);
    QString validationError() const;
    void unsetValidationError();
    bool validationSuccessful() const;

    bool hasBeenUpdated() const;
    bool hasBeenValidated() const;
    DataInformationEndianess byteOrder() const;
    QSysInfo::Endian effectiveByteOrder() const;
    void setByteOrder(DataInformationEndianess newEndianess);
    QString fullObjectPath() const;

    virtual void resetValidationState(); //virtual for DataInformationWithChildren
    bool wasAbleToRead() const;
    /**
     * This method is virtual since DummyDataInformation has to override it.
     *
     * @param engine the script engine
     * @param handlerInfo the object holding the script classes
     * @return a QScriptValue wrapping this object
     */
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    /** the same as above, just using the properties from TopLevelDataInformation */
    QScriptValue toScriptValue(TopLevelDataInformation* top);
    void setParent(DataInformationBase* newParent);
    DataInformationBase* parent() const;
    ScriptLogger* logger() const;
    /** just a shorthand for logger->info(this) */
    QDebug logInfo() const;
    /** just a shorthand for logger->warn(this) */
    QDebug logWarn() const;
    /** just a shorthand for logger->error(this) */
    QDebug logError() const;
    /** whether data was logged from here (and which level it was)
     * @return ScriptLogger::LogInvalid if no data was logged or the level of the most severe log
     */
    ScriptLogger::LogLevel loggedData() const;
    void setLoggedData(ScriptLogger::LogLevel lvl) const;

    /**
     * Find the index of a DataInformation in this object, needed to calculate the row
     * @return the index of @p data or -1 if not found
     */
    virtual int indexOf(const DataInformation* const data) const = 0;
protected:
    bool additionalDataNeeded(AdditionalData* data) const;
    AdditionalData* additionalData() const;
    void setAdditionalData(AdditionalData* data);
    /** So that this object can be wrapped by the correct javascript object*/
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const = 0;
    static QVariant eofReachedData(int role);
private:
    void setValidationError(QString errorMessage); //only called by ScriptHandler
protected:
    bool mValidationSuccessful :1;
    bool mHasBeenValidated :1;
    bool mHasBeenUpdated :1;
    bool mWasAbleToRead :1;
    DataInformationEndianess mByteOrder :2;
    mutable ScriptLogger::LogLevel mLoggedData :2; //mutable is ugly but i guess it is the best solution
    QScopedPointer<AdditionalData> mAdditionalData;
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

inline void DataInformation::setName(const QString& newName)
{
    mName = newName;
}

inline DataInformation* DataInformation::childAt(unsigned int) const
{
    return NULL;
}

inline bool DataInformation::canHaveChildren() const
{
    return false;
}

inline unsigned int DataInformation::childCount() const
{
    return 0;
}

inline bool DataInformation::wasAbleToRead() const
{
    return mWasAbleToRead;
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

inline AdditionalData* DataInformation::additionalData() const
{
    return mAdditionalData.data();
}

inline QDebug DataInformation::logInfo() const
{
    return logger()->info(this);
}

inline QDebug DataInformation::logWarn() const
{
    return logger()->warn(this);
}

inline QDebug DataInformation::logError() const
{
    return logger()->error(this);
}

inline ScriptLogger::LogLevel DataInformation::loggedData() const
{
    return mLoggedData;
}

inline void DataInformation::setLoggedData(ScriptLogger::LogLevel lvl) const
{
    mLoggedData = lvl;
}

inline bool DataInformation::hasBeenUpdated() const
{
    return mHasBeenUpdated;
}

inline bool DataInformation::validationSuccessful() const
{
    return mValidationSuccessful;
}

inline bool DataInformation::hasBeenValidated() const
{
    return mHasBeenValidated;
}

#endif /* DATAINFORMATION_H_ */
