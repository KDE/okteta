/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESTOOLVIEWFACTORY_HPP
#define KASTEN_STRUCTURESTOOLVIEWFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT StructuresToolViewFactory : public AbstractToolViewFactory
{
public:
    StructuresToolViewFactory();
    StructuresToolViewFactory(const StructuresToolViewFactory&) = delete;

    ~StructuresToolViewFactory() override;

    StructuresToolViewFactory& operator=(const StructuresToolViewFactory&) = delete;

public: // AbstractToolViewFactory API
    std::unique_ptr<AbstractToolView> create(AbstractTool* tool) const override;

    QString iconName() const override;
    QString title() const override;
    QString id() const override;
    SidePosition defaultPosition() const override;
};

}

#endif
