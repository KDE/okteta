/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FLAGDATAINFORMATION_HPP
#define KASTEN_FLAGDATAINFORMATION_HPP

#include "enumdatainformation.hpp"

class FlagDataInformation : public EnumDataInformation
{
    DATAINFORMATION_CLONE(FlagDataInformation, EnumDataInformation) {}

public:
    FlagDataInformation(const QString& name, PrimitiveDataInformation* type, const EnumDefinition::Ptr& enumDef,
                        DataInformation* parent = nullptr);

private:
    QString valueStringImpl() const override;
    QString typeNameImpl() const override;
};

#endif // KASTEN_FLAGDATAINFORMATION_HPP
