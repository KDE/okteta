/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMNETSTOOLVIEWFACTORY_HPP
#define KASTEN_DOCUMNETSTOOLVIEWFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class KASTENCONTROLLERS_EXPORT DocumentsToolViewFactory : public AbstractToolViewFactory
{
public:
    DocumentsToolViewFactory();
    DocumentsToolViewFactory(const DocumentsToolViewFactory&) = delete;

    ~DocumentsToolViewFactory() override;

    DocumentsToolViewFactory& operator=(const DocumentsToolViewFactory&) = delete;

public: // AbstractToolViewFactory API
    std::unique_ptr<AbstractToolView> create(AbstractTool* tool) const override;

    QString iconName() const override;
    QString title() const override;
    QString id() const override;
    SidePosition defaultPosition() const override;
};

}

#endif
