/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDOCUMENTFACTORY_HPP
#define KASTEN_BYTEARRAYDOCUMENTFACTORY_HPP

// lib
#include "oktetakastencore_export.hpp"
// Kasten core
#include <Kasten/AbstractDocumentFactory>

namespace Kasten {

class OKTETAKASTENCORE_EXPORT ByteArrayDocumentFactory : public AbstractDocumentFactory
{
    Q_OBJECT

public: // AbstractDocumentFactory API
    bool canCreateFromData(const QMimeData* mimeData) override;

    AbstractDocument* create() override;
    AbstractDocument* createFromData(const QMimeData* mimeData, bool setModified) override;
};

}

#endif
