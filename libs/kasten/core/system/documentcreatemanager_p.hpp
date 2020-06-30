/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTCREATEMANAGER_P_HPP
#define KASTEN_DOCUMENTCREATEMANAGER_P_HPP

// lib
#include "documentcreatemanager.hpp"

namespace Kasten {

class DocumentCreateManagerPrivate
{
public:
    explicit DocumentCreateManagerPrivate(DocumentManager* manager);
    DocumentCreateManagerPrivate(const DocumentCreateManagerPrivate&) = delete;

    ~DocumentCreateManagerPrivate();

public:
    DocumentCreateManagerPrivate& operator=(const DocumentCreateManagerPrivate&) = delete;

public:
    bool canCreateNewFromData(const QMimeData* mimeData) const;

public:
    void createNew() const;
    void createNewFromData(const QMimeData* mimeData, bool setModified) const;

public:
    void setDocumentFactory(AbstractDocumentFactory* factory);

private:
    // unless there is a singleton
    DocumentManager* mManager;

    // temporary hack: hard coded factory for byte arrays
    AbstractDocumentFactory* mFactory = nullptr;
};

}

#endif
