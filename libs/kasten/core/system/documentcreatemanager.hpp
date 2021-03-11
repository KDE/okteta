/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTCREATEMANAGER_HPP
#define KASTEN_DOCUMENTCREATEMANAGER_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>

class QMimeData;

namespace Kasten {

class AbstractDocumentFactory;
class DocumentManager;

class DocumentCreateManagerPrivate;

class KASTENCORE_EXPORT DocumentCreateManager : public QObject
{
    Q_OBJECT

public:
    explicit DocumentCreateManager(DocumentManager* manager);
    DocumentCreateManager() = delete;

    ~DocumentCreateManager() override;

public:
    bool canCreateNewFromData(const QMimeData* mimeData) const;

public:
    void createNew() const;
    void createNewFromData(const QMimeData* mimeData, bool setModified) const;

public:
    void setDocumentFactory(AbstractDocumentFactory* factory);

private:
    const QScopedPointer<class DocumentCreateManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DocumentCreateManager)
};

}

#endif
