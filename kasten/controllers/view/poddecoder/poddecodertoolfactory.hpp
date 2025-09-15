/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODDECODERTOOLFACTORY_HPP
#define KASTEN_PODDECODERTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT PodDecoderToolFactory : public AbstractToolFactory
{
public:
    explicit PodDecoderToolFactory(AbstractUserMessagesHandler* userMessagesHandler);
    PodDecoderToolFactory(const PodDecoderToolFactory&) = delete;
    PodDecoderToolFactory(PodDecoderToolFactory&&) = delete;

    ~PodDecoderToolFactory() override;

    PodDecoderToolFactory& operator=(const PodDecoderToolFactory&) = delete;
    PodDecoderToolFactory& operator=(PodDecoderToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
