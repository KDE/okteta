/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSTOOLFACTORY_HPP
#define KASTEN_DOCUMENTSTOOLFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class DocumentManager;

class KASTENCONTROLLERS_EXPORT DocumentsToolFactory : public AbstractToolFactory
{
public:
    explicit DocumentsToolFactory(DocumentManager* documentManager);
    DocumentsToolFactory(const DocumentsToolFactory&) = delete;
    DocumentsToolFactory(DocumentsToolFactory&&) = delete;

    ~DocumentsToolFactory() override;

    DocumentsToolFactory& operator=(const DocumentsToolFactory&) = delete;
    DocumentsToolFactory& operator=(DocumentsToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;

private:
    DocumentManager* const m_documentManager;
};

}

#endif
