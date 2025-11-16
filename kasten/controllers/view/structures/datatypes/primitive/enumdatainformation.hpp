/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMDATAINFORMATION_HPP
#define KASTEN_ENUMDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"
#include "enumdefinition.hpp"

class EnumDataInformation : public PrimitiveDataInformationWrapper
{
    DATAINFORMATION_CLONE(EnumDataInformation, PrimitiveDataInformationWrapper), mEnum(d.mEnum) {}

public:
    /** this object takes ownership of @p type */
    EnumDataInformation(const QString& name,
                        std::unique_ptr<PrimitiveDataInformation>&& type,
                        const std::shared_ptr<EnumDefinition>& enumDef, DataInformation* parent = nullptr);
    ~EnumDataInformation() override;

public: // DataInformation API
    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isEnum() const override;

public:
    [[nodiscard]]
    const EnumDefinition* enumValues() const;
    /// Not thread-safe
    void setEnumValues(std::map<AllPrimitiveTypes, QString>&& newValues);

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

protected:
    std::shared_ptr<EnumDefinition> mEnum;
};

inline const EnumDefinition* EnumDataInformation::enumValues() const
{
    return mEnum.get();
}

inline void EnumDataInformation::setEnumValues(std::map<AllPrimitiveTypes, QString>&& newValues)
{
    if (mEnum.use_count() == 1) {
        // TODO: is keeping the old name always correct?
        mEnum->setValues(std::move(newValues));
    } else {
        // create new definition instance, unnamed
        mEnum = std::make_shared<EnumDefinition>(std::move(newValues), QString(), mEnum->type());
    }
}
#endif /* KASTEN_ENUMDATAINFORMATION_HPP */
