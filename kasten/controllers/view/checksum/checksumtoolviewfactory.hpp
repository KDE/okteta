/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHECKSUMTOOLVIEWFACTORY_HPP
#define KASTEN_CHECKSUMTOOLVIEWFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT ChecksumToolViewFactory : public AbstractToolViewFactory
{
public:
    ChecksumToolViewFactory();
    ChecksumToolViewFactory(const ChecksumToolViewFactory&) = delete;
    ChecksumToolViewFactory(ChecksumToolViewFactory&&) = delete;

    ~ChecksumToolViewFactory() override;

    ChecksumToolViewFactory& operator=(const ChecksumToolViewFactory&) = delete;
    ChecksumToolViewFactory& operator=(ChecksumToolViewFactory&&) = delete;

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
