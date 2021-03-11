/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TERMINALTOOLFACTORY_HPP
#define KASTEN_TERMINALTOOLFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class DocumentSyncManager;

class KASTENCONTROLLERS_EXPORT TerminalToolFactory : public AbstractToolFactory
{
public:
    explicit TerminalToolFactory(DocumentSyncManager* documentSyncManager);
    ~TerminalToolFactory() override;

public: // AbstractToolFactory API
    AbstractTool* create() const override;

private:
    DocumentSyncManager* const m_documentSyncManager;
};

}

#endif
