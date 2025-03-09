/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTETABLETOOLVIEWFACTORY_HPP
#define KASTEN_BYTETABLETOOLVIEWFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT ByteTableToolViewFactory : public AbstractToolViewFactory
{
public:
    ByteTableToolViewFactory();
    ByteTableToolViewFactory(const ByteTableToolViewFactory&) = delete;
    ByteTableToolViewFactory(ByteTableToolViewFactory&&) = delete;

    ~ByteTableToolViewFactory() override;

    ByteTableToolViewFactory& operator=(const ByteTableToolViewFactory&) = delete;
    ByteTableToolViewFactory& operator=(ByteTableToolViewFactory&&) = delete;

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
