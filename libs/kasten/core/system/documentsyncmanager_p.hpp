/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSYNCMANAGER_P_HPP
#define KASTEN_DOCUMENTSYNCMANAGER_P_HPP

// lib
#include "documentsyncmanager.hpp"
// Std
#include <memory>

class KJob;

namespace Kasten {

class DocumentSyncManagerPrivate
{
public:
    explicit DocumentSyncManagerPrivate(DocumentSyncManager* q, DocumentManager* manager);
    DocumentSyncManagerPrivate(const DocumentSyncManagerPrivate&) = delete;

    ~DocumentSyncManagerPrivate();

    DocumentSyncManagerPrivate& operator=(const DocumentSyncManagerPrivate&) = delete;

public:
    void load(const QUrl& url);
    [[nodiscard]]
    bool setSynchronizer(AbstractDocument* document);
    [[nodiscard]]
    bool canClose(AbstractDocument* document);
    void reload(AbstractDocument* document);
    void save(AbstractDocument* document);

public:
    [[nodiscard]]
    QStringList supportedRemoteTypes() const;
    [[nodiscard]]
    bool hasSynchronizerForLocal(const QString& workDocumentType) const;
    [[nodiscard]]
    QUrl urlOf(AbstractDocument* document) const;

public:
    void setDocumentSynchronizerFactory(std::unique_ptr<AbstractModelSynchronizerFactory>&& synchronizerFactory);
    void setSaveDiscardDialog(AbstractSaveDiscardDialog* saveDiscardDialog);
    void setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog);

private: // slots
    void onDocumentLoadJobResult(KJob* job);

private:
    DocumentSyncManager* const q_ptr;

    // unless there is a singleton
    DocumentManager* const mManager;

    // temporary hack: hard coded factories for byte arrays
    std::unique_ptr<AbstractModelSynchronizerFactory> mSynchronizerFactory;

    AbstractSaveDiscardDialog* mSaveDiscardDialog = nullptr;
    AbstractOverwriteDialog* mOverwriteDialog = nullptr;

    Q_DECLARE_PUBLIC(DocumentSyncManager)
};

}

#endif
