/*
    SPDX-FileCopyrightText: 2006-2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTDOCUMENTFACTORY_HPP
#define KASTEN_ABSTRACTDOCUMENTFACTORY_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>
// Std
#include <memory>

class QMimeData;

namespace Kasten {

class AbstractDocument;

class KASTENCORE_EXPORT AbstractDocumentFactory : public QObject
{
    Q_OBJECT

public:
    ~AbstractDocumentFactory() override;

public: // API to be implemented
    /// default returns false
    [[nodiscard]]
    virtual bool canCreateFromData(const QMimeData* mimeData);

    [[nodiscard]]
    virtual std::unique_ptr<AbstractDocument> create() = 0;
    /// default returns 0
    [[nodiscard]]
    virtual std::unique_ptr<AbstractDocument> createFromData(const QMimeData* mimeData, bool setModified);
};

}

#endif
