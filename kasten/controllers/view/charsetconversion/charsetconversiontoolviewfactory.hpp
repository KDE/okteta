/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONTOOLVIEWFACTORY_HPP
#define KASTEN_CHARSETCONVERSIONTOOLVIEWFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT CharsetConversionToolViewFactory : public AbstractToolViewFactory
{
public:
    explicit CharsetConversionToolViewFactory(AbstractUserMessagesHandler* userMessagesHandler);
    CharsetConversionToolViewFactory(const CharsetConversionToolViewFactory&) = delete;
    CharsetConversionToolViewFactory(CharsetConversionToolViewFactory&&) = delete;

    ~CharsetConversionToolViewFactory() override;

    CharsetConversionToolViewFactory& operator=(const CharsetConversionToolViewFactory&) = delete;
    CharsetConversionToolViewFactory& operator=(CharsetConversionToolViewFactory&&) = delete;

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

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
