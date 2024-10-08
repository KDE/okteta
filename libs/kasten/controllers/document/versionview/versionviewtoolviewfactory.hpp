/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONVIEWTOOLVIEWFACTORY_HPP
#define KASTEN_VERSIONVIEWTOOLVIEWFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class KASTENCONTROLLERS_EXPORT VersionViewToolViewFactory : public AbstractToolViewFactory
{
public:
    VersionViewToolViewFactory();
    VersionViewToolViewFactory(const VersionViewToolViewFactory&) = delete;

    ~VersionViewToolViewFactory() override;

    VersionViewToolViewFactory& operator=(const VersionViewToolViewFactory&) = delete;

public: // AbstractToolViewFactory API
    std::unique_ptr<AbstractToolView> create(AbstractTool* tool) const override;

    QString iconName() const override;
    QString title() const override;
    QString id() const override;
    SidePosition defaultPosition() const override;
};

}

#endif
