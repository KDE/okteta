/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "enumdatainformation.hpp"
#include "../topleveldatainformation.hpp"

#include <KComboBox>
#include <KLocalizedString>
#include <QScriptValue>

#include "../../script/classes/enumscriptclass.hpp"
#include "../../script/scriptlogger.hpp"

EnumDataInformation::EnumDataInformation(const QString& name, PrimitiveDataInformation* type,
                                         const EnumDefinition::Ptr& enumDef, DataInformation* parent)
    : PrimitiveDataInformationWrapper(name, type, parent)
    , mEnum(enumDef)
{
    Q_CHECK_PTR(type);
    if (enumDef->type() != type->type()) {
        logWarn() << "incompatible types in definition and value: "
                  << enumDef->type() << "and " << type->type();
    }
    mValue->setParent(this);
}

EnumDataInformation::~EnumDataInformation() = default;

QWidget* EnumDataInformation::createEditWidget(QWidget* parent) const
{
    auto* box = new KComboBox(false, parent);

    const QMap<AllPrimitiveTypes, QString>& enumValues = mEnum->values();

    // TODO: have an option to edit custom values not defined in the enum
    if (!enumValues.contains(mValue->value())) {
        const QString text = i18n("%1 (value not in enum)", mValue->valueString());
        box->addItem(text, mValue->valueToQVariant());
        box->insertSeparator(1);
    }

    for (auto it = enumValues.begin(), end = enumValues.end(); it != end; ++it) {
        const AllPrimitiveTypes data = it.key();
        const QString& enumVal = it.value();
        const QString text = i18n("%1 (%2)", enumVal, mValue->valueToQString(data));
        box->addItem(text, mValue->valueToQVariant(data));
    }
    return box;
}

QVariant EnumDataInformation::dataFromWidget(const QWidget* w) const
{
    const auto* box = qobject_cast<const KComboBox*>(w);
    Q_CHECK_PTR(box);
    return box->currentData();
}

void EnumDataInformation::setWidgetData(QWidget* w) const
{
    auto* box = qobject_cast<KComboBox*>(w);
    Q_CHECK_PTR(box);
    const int dataIndex = box->findData(mValue->valueToQVariant());
    if (dataIndex != -1) {
        box->setCurrentIndex(dataIndex);
    }
}

QString EnumDataInformation::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    QString enumVal = mEnum->value(mValue->value());
    if (!enumVal.isEmpty()) {
        return i18n("%1 (%2)", enumVal, mValue->valueString());
    }

    return i18n("%1 (value not in enum)", mValue->valueString());
}

QString EnumDataInformation::typeNameImpl() const
{
    return i18nc("Displayed in the type column. first comes the name "
                 "of the enum, then the underlying type (e.g. uint32)",
                 "enum %1 (%2)", mEnum->name(), mValue->typeName());
}

QScriptClass* EnumDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mEnumClass.get();
}

bool EnumDataInformation::isEnum() const
{
    return true;
}
