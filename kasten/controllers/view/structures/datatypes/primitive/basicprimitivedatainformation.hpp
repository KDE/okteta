/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BASICPRIMITIVEDATAINFORMATION_HPP
#define KASTEN_BASICPRIMITIVEDATAINFORMATION_HPP

#include "primitivedatainformation.hpp"
#include "../../allprimitivetypes.hpp"

/** A basic implementation for all primitive types.
 *  T is the data type (e.g. quint8 or float)
 *  C is a namespace containing all needed functions */
template <typename T, class C>
class BasicPrimitiveDataInformation : public PrimitiveDataInformation
{
public:
    BasicPrimitiveDataInformation(const QString& name, DataInformation* parent = nullptr);
    ~BasicPrimitiveDataInformation() override;
    PrimitiveDataType type() const override;
    AllPrimitiveTypes value() const override;
    void setValue(AllPrimitiveTypes newVal) override;
    BasicPrimitiveDataInformation<T, C>* clone() const override;

    QWidget* createEditWidget(QWidget* parent) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

    QScriptValue valueAsQScriptValue() const override;
    BitCount32 size() const override; // TODO declare final with c++11 so it can be inlined
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;
    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;

protected:
    BasicPrimitiveDataInformation(const BasicPrimitiveDataInformation<T, C>& d);

private:
    QString valueStringImpl() const override;
    QString typeNameImpl() const override;
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

protected:
    T mValue = 0;
};

template <typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>::BasicPrimitiveDataInformation(const QString& name,
                                                                          DataInformation* parent)
    : PrimitiveDataInformation(name, parent)
{
}

template <typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>::BasicPrimitiveDataInformation(
    const BasicPrimitiveDataInformation<T, C>& d)
    : PrimitiveDataInformation(d)
    , mValue(d.mValue)
{
}

template <typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>::~BasicPrimitiveDataInformation() = default;

template <typename T, typename C>
inline QString BasicPrimitiveDataInformation<T, C>::typeNameImpl() const
{
    return PrimitiveType::typeName(C::staticType());
}

template <typename T, typename C>
inline AllPrimitiveTypes BasicPrimitiveDataInformation<T, C>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template <typename T, typename C>
inline void BasicPrimitiveDataInformation<T, C>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.value<T>();

}

template <typename T, typename C>
inline BitCount32 BasicPrimitiveDataInformation<T, C>::size() const
{
    return sizeof(T) * 8;
}

template <typename T, typename C>
inline QVariant BasicPrimitiveDataInformation<T, C>::dataFromWidget(const QWidget* w) const
{
    return C::staticDataFromWidget(w);
}

template <typename T, typename C>
inline QWidget* BasicPrimitiveDataInformation<T, C>::createEditWidget(QWidget* parent) const
{
    return C::staticCreateEditWidget(parent);
}

template <typename T, typename C>
inline void BasicPrimitiveDataInformation<T, C>::setWidgetData(QWidget* w) const
{
    C::staticSetWidgetData(mValue, w);
}

template <typename T, typename C>
inline PrimitiveDataType BasicPrimitiveDataInformation<T, C>::type() const
{
    return C::staticType();
}

template <typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>* BasicPrimitiveDataInformation<T, C>::clone() const
{
    return new BasicPrimitiveDataInformation<T, C>(*this);
}

template <typename T, typename C>
QString BasicPrimitiveDataInformation<T, C>::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    return C::staticValueString(mValue);
}

#endif /* KASTEN_BASICPRIMITIVEDATAINFORMATION_HPP */
