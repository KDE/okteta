/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TERMINALTOOLVIEWFACTORY_HPP
#define KASTEN_TERMINALTOOLVIEWFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class KASTENCONTROLLERS_EXPORT TerminalToolViewFactory : public AbstractToolViewFactory
{
public:
    TerminalToolViewFactory();
    TerminalToolViewFactory(const TerminalToolViewFactory&) = delete;
    TerminalToolViewFactory(TerminalToolViewFactory&&) = delete;

    ~TerminalToolViewFactory() override;

    TerminalToolViewFactory& operator=(const TerminalToolViewFactory&) = delete;
    TerminalToolViewFactory& operator=(TerminalToolViewFactory&&) = delete;

public: // AbstractToolViewFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractToolView> create(AbstractTool* tool) const override;

    [[nodiscard]]
    QString iconName() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    QString id() const override;
    [[nodiscard]]
    SidePosition defaultPosition() const override;
};

}

#endif
