/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DATAINFORMATION_HPP
#define KASTEN_DATAINFORMATION_HPP

// lib
#include "datainformationbase.hpp"
#include "primitivedatatype.hpp"
#include "additionaldata.hpp"
#include <scriptlogger.hpp>
// Okteta core
#include <Okteta/Size>
#include <Okteta/Address>
// Qt
#include <QString>
#include <QMetaType>
// Std
#include <memory>

/** Implement the cloneImpl() method and add the copy constructor declaration
 * After this macro visibility will be set to protected */
#define DATAINFORMATION_CLONE_DECL(type, supertype) \
    private: \
        [[nodiscard]] \
        inline type* cloneImpl() const override { return new type(*this); } \
    protected: \
        type(const type& d)

/** Implements the clone() method and add the start of the copy constructor (up to delegating).
 *  After this macro visibility will be set to protected */
#define DATAINFORMATION_CLONE(type, supertype) DATAINFORMATION_CLONE_DECL(type, supertype) : supertype(d)

namespace Okteta {
class AbstractByteArrayModel;
}

class QScriptEngine;
class QScriptValue;
class QScriptClass;
class QVariant;
class QWidget;
class ScriptLogger;
class TopLevelDataInformation;
class ScriptHandlerInfo;

/** Interface that must be implemented by all datatypes */
class DataInformation : public DataInformationBase
{
    friend class ScriptHandler; // so mHasBeenUpdated/hasBeenValidated can be set
    friend class PrimitiveDataInformationWrapper; // to set mHasBeenUpdated
    friend class PointerDataInformation; // to set mWasAbleToRead on pointer target
    friend class DefaultScriptClass; // to set mValidationSucessful and validationError
    template <PrimitiveDataType type> friend class PrimitiveArrayData; // to set mWasAbleToRead (when returning a script value)

public:
    enum Columns
    {
        ColumnName = 0,
        ColumnType,
        ColumnValue,
        ColumnSize,
        COLUMN_COUNT
    };
    enum class DataInformationEndianness
    {
        EndiannessFromSettings = 0,
        EndiannessInherit,
        EndiannessLittle,
        EndiannessBig
    };

protected:
    explicit DataInformation(const DataInformation&);

public:
    explicit DataInformation(const QString& name, DataInformationBase* parent = nullptr);
    ~DataInformation() override;

public: // API to implement
    // methods for children:
    [[nodiscard]]
    virtual unsigned int childCount() const = 0;
    [[nodiscard]]
    virtual DataInformation* childAt(unsigned int) const = 0;
    /**
     * @param child the direct child we want to find the address for
     * @param start the start of the root element
     * @return the address of @p child in the file
     */
    [[nodiscard]]
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const = 0;

    // for the model:
    [[nodiscard]]
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    /** get the necessary data (for the model) */
    [[nodiscard]]
    virtual QVariant data(int column, int role) const;

    [[nodiscard]]
    virtual QString tooltipString() const;
    /** needs to be virtual for bitfields */
    [[nodiscard]]
    virtual QString sizeString() const;

    // delegate functions:
    /** create a QWidget for the QItemDelegate */
    [[nodiscard]]
    virtual QWidget* createEditWidget(QWidget* parent) const = 0;
    /** get the needed data from the widget */
    [[nodiscard]]
    virtual QVariant dataFromWidget(const QWidget* w) const = 0;
    /** initialize the delegate widget with the correct data */
    virtual void setWidgetData(QWidget* w) const = 0;

    // reading and writing
    /** the size in bits of this element */
    [[nodiscard]]
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
    [[nodiscard]]
    virtual qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                            BitCount64 bitsRemaining, quint8* bitOffset) = 0;
    /** Writes the current data contained in this object to out.
     *
     *  @param value a QVariant object holding the new data to write
     *  @param out the byte array the value is written to
     *  @param address the address in @p out
     *  @param bitsRemaining number of bits remaining in @p out
     *  @param bitOffset the bit to start at in the first byte
     *
     *  @return @c true on success, @c false otherwise
     */
    [[nodiscard]]
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                         Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) = 0;

    virtual void resetValidationState(); // virtual for DataInformationWithChildren

    /**
     * This method is virtual since DummyDataInformation has to override it.
     *
     * @param engine the script engine
     * @param handlerInfo the object holding the script classes
     * @return a QScriptValue wrapping this object
     */
    [[nodiscard]]
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    /**
     * Find the index of a DataInformation in this object, needed to calculate the row
     * @return the index of @p data or -1 if not found
     */
    [[nodiscard]]
    virtual int indexOf(const DataInformation* const data) const = 0;

public: // DataInformationBase API
    [[nodiscard]]
    bool isTopLevel() const override;

public:
    /**
     * Redefine in subclasses where covariance of return type is wanted.
     * See also cloneImpl() for actual per-class-specific cloning.
     */
    [[nodiscard]]
    std::unique_ptr<DataInformation> clone() const;

    [[nodiscard]]
    int row() const;
    [[nodiscard]]
    QString typeName() const;
    /** by default just returns an empty QString */
    [[nodiscard]]
    QString valueString() const;
    [[nodiscard]]
    QString name() const;
    void setName(const QString& newName);

    /** @brief Looks for a child of this object with given name
     *  @param name The name of the child to find
     *  @return the child with given @p name or @c NULL if none found with that name
     */
    [[nodiscard]]
    DataInformation* child(const QString& name) const;
    /**
     * @param start the starting address of the whole structure
     * @return the position in the file where this element is located
     */
    [[nodiscard]]
    BitCount64 positionInFile(Okteta::Address start) const;

    /** sets mWasAbleToRead to false for all children and this object.
     *  Gets called once before the reading of the whole structure starts. */
    void beginRead();
    [[nodiscard]]
    TopLevelDataInformation* topLevelDataInformation() const;
    [[nodiscard]]
    DataInformation* mainStructure();

    [[nodiscard]]
    QScriptValue updateFunc() const;
    void setUpdateFunc(const QScriptValue& func);
    [[nodiscard]]
    QScriptValue validationFunc() const;
    void setValidationFunc(const QScriptValue& func);
    [[nodiscard]]
    QString validationError() const;
    bool validationSuccessful() const;

    [[nodiscard]]
    bool hasBeenUpdated() const;
    [[nodiscard]]
    bool hasBeenValidated() const;
    [[nodiscard]]
    DataInformationEndianness byteOrder() const;
    [[nodiscard]]
    QSysInfo::Endian effectiveByteOrder() const;
    void setByteOrder(DataInformationEndianness newByteOrder);
    [[nodiscard]]
    QString fullObjectPath() const;

    [[nodiscard]]
    bool wasAbleToRead() const;
    /** the same as above, just using the properties from TopLevelDataInformation */
    [[nodiscard]]
    QScriptValue toScriptValue(TopLevelDataInformation* top);
    void setParent(DataInformationBase* newParent);
    [[nodiscard]]
    DataInformationBase* parent() const;
    [[nodiscard]]
    ScriptLogger* logger() const;
    /** just a shorthand for logger->info(this) */
    [[nodiscard]]
    QDebug logInfo() const;
    /** just a shorthand for logger->warn(this) */
    [[nodiscard]]
    QDebug logWarn() const;
    /** just a shorthand for logger->error(this) */
    [[nodiscard]]
    QDebug logError() const;
    /** whether data was logged from here (and which level it was)
     * @return ScriptLogger::LogInvalid if no data was logged or the level of the most severe log
     */
    [[nodiscard]]
    ScriptLogger::LogLevel loggedData() const;
    void setLoggedData(ScriptLogger::LogLevel lvl) const;

    /** Set a custom string to be used for typeName() instead of the default.
     * @param customTypeName the new name. Pass an empty or null string to revert to default behaviour
     */
    void setCustomTypeName(const QString& customTypeName);
    [[nodiscard]]
    QScriptValue toStringFunction() const;
    void setToStringFunction(const QScriptValue& func);

protected:
    [[nodiscard]]
    static QString sizeString(BitCount32 size);
    [[nodiscard]]
    static QString tooltipString(const QString& nameString, const QString& valueString,
                                 const QString& typeString, const QString& sizeString,
                                 unsigned int childCount = 0,
                                 const QString& validationMessage = QString());
    [[nodiscard]]
    static QVariant eofReachedData(int role);
    void setAdditionalFunction(AdditionalData::AdditionalDataType entry, const QScriptValue& value, const char* name);

private: // API to implement
    [[nodiscard]]
    virtual DataInformation* cloneImpl() const = 0;

    [[nodiscard]]
    virtual QString typeNameImpl() const = 0;
    [[nodiscard]]
    virtual QString valueStringImpl() const;
    /** So that this object can be wrapped by the correct javascript object*/
    [[nodiscard]]
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const = 0;

private:
    void setValidationError(const QString& errorMessage); // only called by ScriptHandler
    [[nodiscard]]
    QSysInfo::Endian byteOrderFromSettings() const; // so there is no need to include structureviewpreferences.hpp here
    [[nodiscard]]
    QString customToString(const QScriptValue& func) const;

protected:
    AdditionalData mAdditionalData;
    DataInformationBase* mParent = nullptr;
    QString mName;
    bool mValidationSuccessful : 1;
    bool mHasBeenValidated : 1;
    bool mHasBeenUpdated : 1;
    bool mWasAbleToRead : 1;
    bool mWasAbleToReadBefore : 1;
    DataInformationEndianness mByteOrder = DataInformationEndianness::EndiannessInherit;
    mutable ScriptLogger::LogLevel mLoggedData : 2; // mutable is ugly but i guess it is the best solution
};

Q_DECLARE_METATYPE(DataInformation*)
Q_DECLARE_METATYPE(const DataInformation*)

inline std::unique_ptr<DataInformation> DataInformation::clone() const
{
    return std::unique_ptr<DataInformation>(cloneImpl());
}

inline Qt::ItemFlags DataInformation::flags(int column, bool fileLoaded) const
{
    Q_UNUSED(column)
    Q_UNUSED(fileLoaded)
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
    return nullptr;
}

inline unsigned int DataInformation::childCount() const
{
    return 0;
}

inline bool DataInformation::wasAbleToRead() const
{
    return mWasAbleToRead;
}

inline DataInformation::DataInformationEndianness DataInformation::byteOrder() const
{
    return mByteOrder;
}

inline void DataInformation::setByteOrder(DataInformation::DataInformationEndianness newByteOrder)
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

inline QScriptValue DataInformation::updateFunc() const
{
    return mAdditionalData.get(AdditionalData::AdditionalDataType::UpdateFunction).value<QScriptValue>();
}

inline QScriptValue DataInformation::validationFunc() const
{
    return mAdditionalData.get(AdditionalData::AdditionalDataType::ValidationFunction).value<QScriptValue>();
}

inline QScriptValue DataInformation::toStringFunction() const
{
    return mAdditionalData.get(AdditionalData::AdditionalDataType::ToStringFunction).value<QScriptValue>();
}

inline void DataInformation::setUpdateFunc(const QScriptValue& func)
{
    setAdditionalFunction(AdditionalData::AdditionalDataType::UpdateFunction, func, "update function");
}

inline void DataInformation::setValidationFunc(const QScriptValue& func)
{
    setAdditionalFunction(AdditionalData::AdditionalDataType::ValidationFunction, func, "validation function");
}

inline void DataInformation::setToStringFunction(const QScriptValue& func)
{
    setAdditionalFunction(AdditionalData::AdditionalDataType::ToStringFunction, func, "to string function");
}

inline QString DataInformation::validationError() const
{
    return mAdditionalData.get(AdditionalData::AdditionalDataType::ValidationError).toString();
}

inline QSysInfo::Endian DataInformation::effectiveByteOrder() const
{
    switch (mByteOrder)
    {
    case DataInformationEndianness::EndiannessBig:
        return QSysInfo::BigEndian;
    case DataInformationEndianness::EndiannessLittle:
        return QSysInfo::LittleEndian;
    case DataInformationEndianness::EndiannessFromSettings:
        return byteOrderFromSettings();
    default: // inherit
        if (mParent && !mParent->isTopLevel()) {
            return mParent->asDataInformation()->effectiveByteOrder();
        }
        return byteOrderFromSettings();
    }
}

inline QString DataInformation::typeName() const
{
    QVariant v = mAdditionalData.get(AdditionalData::AdditionalDataType::CustomTypeName);
    if (Q_UNLIKELY(v.isValid())) { // custom type names will be used rarely
        return v.toString();
    }
    return typeNameImpl();
}

inline QString DataInformation::valueString() const
{
    QVariant v = mAdditionalData.get(AdditionalData::AdditionalDataType::ToStringFunction);
    if (Q_UNLIKELY(v.isValid())) { // custom to string functions will be used rarely
        return customToString(v.value<QScriptValue>());
    }
    return valueStringImpl();
}

#endif /* KASTEN_DATAINFORMATION_HPP */
