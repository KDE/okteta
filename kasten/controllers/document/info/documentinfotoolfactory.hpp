/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTINFOTOOLFACTORY_HPP
#define KASTEN_DOCUMENTINFOTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class DocumentSyncManager;

class OKTETAKASTENCONTROLLERS_EXPORT DocumentInfoToolFactory : public AbstractToolFactory
{
public:
    explicit DocumentInfoToolFactory(DocumentSyncManager* syncManager);
    DocumentInfoToolFactory(const DocumentInfoToolFactory&) = delete;

    ~DocumentInfoToolFactory() override;

    DocumentInfoToolFactory& operator=(const DocumentInfoToolFactory&) = delete;

public: // AbstractToolFactory API
    AbstractTool* create() const override;

private:
    DocumentSyncManager* const m_syncManager;
};

}

#endif
