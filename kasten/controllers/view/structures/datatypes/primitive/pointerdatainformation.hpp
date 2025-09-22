/*
    SPDX-FileCopyrightText: 2012 Alessandro Di Federico <ale@clearmind.me>
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardosn@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_POINTERDATAINFORMATION_HPP
#define KASTEN_POINTERDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"
// Std
#include <memory>

class PointerDataInformation : public PrimitiveDataInformationWrapper
{
    DATAINFORMATION_CLONE_DECL(PointerDataInformation, PrimitiveDataInformationWrapper);

public:
    /** creates a new pointer
     *  takes ownership over @p pointerTarget and @p valueType
     */
    PointerDataInformation(const QString& name,
                           std::unique_ptr<DataInformation>&& pointerTarget,
                           PrimitiveDataInformation* valueType, DataInformation* parent,
                           qint64 pointerScale, const QScriptValue& interpretFunction);
    ~PointerDataInformation() override;

public: // DataInformation API
    [[nodiscard]]
    uint childCount() const override;
    [[nodiscard]]
    DataInformation* childAt(uint index) const override;
    [[nodiscard]]
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;

    [[nodiscard]]
    int indexOf(const DataInformation* const data) const override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isPointer() const override;

public:
    /** Called once the whole structure has been read. Now we can evaluate what we are pointing at.
     * @param input the input
     * @param address the address of the root structure start */
    void delayedReadData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address);

    [[nodiscard]]
    DataInformation* pointerTarget() const;
    /** Set a new pointer target
     * @param pointerTarget the new target (ownership is taken)
     */
    void setPointerTarget(std::unique_ptr<DataInformation>&& pointerTarget);

    [[nodiscard]]
    PrimitiveDataInformation* pointerType() const;
    /** Set a new pointer target
     * @param type the new pointer type (ownership is taken if return value is true)
     * @return true if type was set, false if not
     */
    [[nodiscard]]
    bool setPointerType(DataInformation* type);

    [[nodiscard]]
    qint64 pointerScale() const;
    /** Set a new pointer scale
     * @param scale the new pointer scale, in bytes.
     */
    void setPointerScale(qint64 scale);

    [[nodiscard]]
    QScriptValue interpreterFunction() const;
    void setInterpreterFunction(const QScriptValue& newFunc);

    [[nodiscard]]
    quint64 interpret(Okteta::Address start) const;

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

protected:
    std::unique_ptr<DataInformation> mPointerTarget;
    qint64 mPointerScale;
};

inline DataInformation* PointerDataInformation::pointerTarget() const
{
    return mPointerTarget.get();
}

inline void PointerDataInformation::setPointerTarget(std::unique_ptr<DataInformation>&& pointerTarget)
{
    Q_CHECK_PTR(pointerTarget);
    mPointerTarget = std::move(pointerTarget);
    mPointerTarget->setParent(this);
}

inline PrimitiveDataInformation* PointerDataInformation::pointerType() const
{
    return mValue.get();
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
