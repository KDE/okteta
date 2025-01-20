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
// Std
#include <memory>

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

    ~DocumentSyncManager() override;

public:
    void load(const QUrl& url);
// TODO: better name
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

Q_SIGNALS:
    void urlUsed(const QUrl& url);

private:
    const std::unique_ptr<class DocumentSyncManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DocumentSyncManager)
};

}

#endif
