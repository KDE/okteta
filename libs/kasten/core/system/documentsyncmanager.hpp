/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSYNCMANAGER_HPP
#define KASTEN_DOCUMENTSYNCMANAGER_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>

class QUrl;
class QString;

namespace Kasten {

class AbstractDocument;
class AbstractModelSynchronizerFactory;
class DocumentManager;
class AbstractSaveDiscardDialog;
class AbstractOverwriteDialog;

class DocumentSyncManagerPrivate;

class KASTENCORE_EXPORT DocumentSyncManager : public QObject
{
    Q_OBJECT

public:
    explicit DocumentSyncManager(DocumentManager* manager);

    DocumentSyncManager() = delete;
    ~DocumentSyncManager() override;

public:
    void load(const QUrl& url);
// TODO: better name
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

Q_SIGNALS:
    void urlUsed(const QUrl& url);

private:
    const QScopedPointer<class DocumentSyncManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DocumentSyncManager)
};

}

#endif
