/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVEDATAINFORMATION_HPP
#define KASTEN_PRIMITIVEDATAINFORMATION_HPP

// lib
#include <datainformation.hpp>
#include <primitivedatatype.hpp>
#include <allprimitivetypes.hpp>
// Std
#include <memory>

union AllPrimitiveTypes;

/**
 * A base class for all primitive data elements (e.g. unsigned integers or floating-point numbers)
 */
class PrimitiveDataInformation : public DataInformation
{
    friend class PrimitiveDataInformationTest; // that unit test needs to change mWasAbleToRead

protected:
    PrimitiveDataInformation(const PrimitiveDataInformation& d);

public:
    explicit PrimitiveDataInformation(const QString& name, DataInformation* parent = nullptr);
    ~PrimitiveDataInformation() override;

public: // API to implement
    [[nodiscard]]
    virtual PrimitiveDataType type() const = 0;

    [[nodiscard]]
    virtual AllPrimitiveTypes value() const = 0;
    virtual void setValue(AllPrimitiveTypes newValue) = 0;

    [[nodiscard]]
    virtual QVariant valueToQVariant() const = 0;
    [[nodiscard]]
    virtual QScriptValue valueAsQScriptValue() const = 0;
    [[nodiscard]]
    virtual QString valueToQString(AllPrimitiveTypes value) const = 0;
    [[nodiscard]]
    virtual QVariant valueToQVariant(AllPrimitiveTypes value) const = 0;

public: // DataInformation API
    [[nodiscard]]
    PrimitiveDataInformation* clone() const override = 0;

    [[nodiscard]]
    unsigned int childCount() const override;
    [[nodiscard]]
    DataInformation* childAt(unsigned int) const override;
    [[nodiscard]]
    BitCount64 childPosition(const DataInformation*, Okteta::Address) const override;

    [[nodiscard]]
    Qt::ItemFlags flags(int column, bool fileLoaded = true) const override;

    [[nodiscard]]
    int indexOf(const DataInformation* const) const override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isPrimitive() const override;

public:
    /** @return the matching prefix for the base (nothing, '0x', '0b' or '0o') */
    [[nodiscard]]
    static QString basePrefix(int base);

protected: // API to implement
    [[nodiscard]]
    virtual BitCount32 offset(unsigned int index) const;
};

/**
 * A base class for data types which just wrap an underlying primitive data type.
 * The basic operations (reading/writing/size/value) are simply delegated to the wrapped element.
 *
 * This is used for pointers (wrapping differently sized integers) and enumerations/bitflags
 * Due to the use of a PrimitiveDataInformation pointer as a member any primitive type can be wrapped.
 * For example pointers using a bitfield value are possible and theoretically floating point enums
 * (although these were not added due to float comparison issues - use the binary representation of an
 * equally sized integer instead).
 */
class PrimitiveDataInformationWrapper : public PrimitiveDataInformation
{
protected:
    PrimitiveDataInformationWrapper(const PrimitiveDataInformationWrapper& d);

public:
    /** takes ownership of @p valueType */
    PrimitiveDataInformationWrapper(const QString& name,
                                    std::unique_ptr<PrimitiveDataInformation>&& valueType,
                                    DataInformation* parent = nullptr);
    ~PrimitiveDataInformationWrapper() override = default;

    // delegate all these to the underlying object:
public: // PrimitiveDataInformation API
    PrimitiveDataType type() const override;

    AllPrimitiveTypes value() const override;
    void setValue(AllPrimitiveTypes newValue) override;

    QVariant valueToQVariant() const override;
    QScriptValue valueAsQScriptValue() const override;
    QString valueToQString(AllPrimitiveTypes value) const override;
    QVariant valueToQVariant(AllPrimitiveTypes value) const override;

public: // DataInformation API
    BitCount32 size() const override;

    void setWidgetData(QWidget* w) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    QWidget* createEditWidget(QWidget* parent) const override;

    // we have to do slightly more than just forward with this method
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;

    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

protected:
    std::unique_ptr<PrimitiveDataInformation> mValue;
};

inline PrimitiveDataInformation::PrimitiveDataInformation(const QString& name,
                                                          DataInformation* parent)
    : DataInformation(name, parent)
{
}

inline PrimitiveDataInformation::PrimitiveDataInformation(const PrimitiveDataInformation& d) = default;

inline QString PrimitiveDataInformation::basePrefix(int base)
{
    switch (base) {
    case 10: return {};
    case 2: return QStringLiteral("0b");
    case 8: return QStringLiteral("0o");
    case 16: return QStringLiteral("0x");
    default:
        Q_ASSERT_X(false, "PrimitiveDataInformation::basePrefix()", "Invalid argument!");
        return {};
    }
}

#endif /* KASTEN_PRIMITIVEDATAINFORMATION_HPP */
