/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alessandro Di Federico <ale@clearmind.me>
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardosn@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_POINTERDATAINFORMATION_HPP
#define KASTEN_POINTERDATAINFORMATION_HPP

#include "primitivedatainformation.hpp"

class PointerDataInformation : public PrimitiveDataInformationWrapper
{
    DATAINFORMATION_CLONE_DECL(PointerDataInformation, PrimitiveDataInformationWrapper);

public:
    /** creates a new pointer
     *  takes ownership over @p childType and @p valueType
     */
    PointerDataInformation(const QString& name, DataInformation* childType,
                           PrimitiveDataInformation* valueType, DataInformation* parent,
                           qint64 pointerScale, const QScriptValue& interpretFunction);
    ~PointerDataInformation() override;

    bool canHaveChildren() const override;
    uint childCount() const override;
    DataInformation* childAt(uint index) const override;
    bool isPointer() const override;
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;
    int indexOf(const DataInformation* const data) const override;
    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    /** Called once the whole structure has been read. Now we can evaluate what we are pointing at.
     * @param input the input
     * @param address the address of the root structure start */
    void delayedReadData(Okteta::AbstractByteArrayModel* input, Okteta::Address address);

    DataInformation* pointerTarget() const;
    /** Set a new pointer target
     * @param target the new target (ownership is taken)
     */
    void setPointerTarget(DataInformation* target);

    PrimitiveDataInformation* pointerType() const;
    /** Set a new pointer target
     * @param type the new pointer type (ownership is taken if return value is true)
     * @return true if type was set, false if not
     */
    bool setPointerType(DataInformation* type);

    qint64 pointerScale() const;
    /** Set a new pointer scale
     * @param scale the new pointer scale, in bytes.
     */
    void setPointerScale(qint64 scale);

    QScriptValue interpreterFunction() const;
    void setInterpreterFunction(const QScriptValue& newFunc);

    quint64 interpret(Okteta::Address start) const;

private:
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;
    QString valueStringImpl() const override;
    QString typeNameImpl() const override;

protected:
    QScopedPointer<DataInformation> mPointerTarget;
    qint64 mPointerScale;
};

inline DataInformation* PointerDataInformation::pointerTarget() const
{
    return mPointerTarget.data();
}

inline void PointerDataInformation::setPointerTarget(DataInformation* target)
{
    Q_CHECK_PTR(target);
    mPointerTarget.reset(target);
    mPointerTarget->setParent(this);
}

inline PrimitiveDataInformation* PointerDataInformation::pointerType() const
{
    return mValue.data();
}

inline void PointerDataInformation::setPointerScale(qint64 scale)
{
    mPointerScale = scale;
}

inline qint64 PointerDataInformation::pointerScale() const
{
    return mPointerScale;
}

inline QScriptValue PointerDataInformation::interpreterFunction() const
{
    return mAdditionalData.get(AdditionalData::AdditionalDataType::PointerInterpreterFunction).value<QScriptValue>();
}

inline void PointerDataInformation::setInterpreterFunction(const QScriptValue& newFunc)
{
    setAdditionalFunction(AdditionalData::AdditionalDataType::PointerInterpreterFunction, newFunc, "pointer interpreter function");
}

#endif // KASTEN_POINTERDATAINFORMATION_HPP
