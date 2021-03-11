/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTINFOTOOLVIEWFACTORY_HPP
#define KASTEN_DOCUMENTINFOTOOLVIEWFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT DocumentInfoToolViewFactory : public AbstractToolViewFactory
{
public:
    DocumentInfoToolViewFactory();
    ~DocumentInfoToolViewFactory() override;

public: // AbstractToolViewFactory API
    AbstractToolView* create(AbstractTool* tool) const override;

    QString iconName() const override;
    QString title() const override;
    QString id() const override;
    SidePosition defaultPosition() const override;
};

}

#endif
