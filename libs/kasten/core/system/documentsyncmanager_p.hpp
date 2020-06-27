/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSYNCMANAGER_P_HPP
#define KASTEN_DOCUMENTSYNCMANAGER_P_HPP

// lib
#include "documentsyncmanager.hpp"

namespace Kasten {

class DocumentSyncManagerPrivate
{
public:
    explicit DocumentSyncManagerPrivate(DocumentSyncManager* q, DocumentManager* manager);

    ~DocumentSyncManagerPrivate();

public:
    void load(const QUrl& url);
    bool setSynchronizer(AbstractDocument* document);
    bool canClose(AbstractDocument* document);
    void reload(AbstractDocument* document);
    void save(AbstractDocument* document);

public:
    QStringList supportedRemoteTypes() const;
    bool hasSynchronizerForLocal(const QString& mimeType) const;
    QUrl urlOf(AbstractDocument* document) const;

public:
    void setDocumentSynchronizerFactory(AbstractModelSynchronizerFactory* synchronizerFactory);
    void setSaveDiscardDialog(AbstractSaveDiscardDialog* saveDiscardDialog);
    void setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog);

private: // slots
    void onDocumentLoaded(Kasten::AbstractDocument* document);

private:
    DocumentSyncManager* const q_ptr;

    // unless there is a singleton
    DocumentManager* mManager;

    // temporary hack: hard coded factories for byte arrays
    AbstractModelSynchronizerFactory* mSynchronizerFactory = nullptr;

    AbstractSaveDiscardDialog* mSaveDiscardDialog = nullptr;
    AbstractOverwriteDialog* mOverwriteDialog = nullptr;

    Q_DECLARE_PUBLIC(DocumentSyncManager)
};

}

#endif
