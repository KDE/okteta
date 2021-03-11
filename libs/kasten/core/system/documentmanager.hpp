/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTMANAGER_HPP
#define KASTEN_DOCUMENTMANAGER_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QVector>
#include <QObject>

class QStringList;

namespace Kasten {

class AbstractDocument;

class DocumentCreateManager; // TODO: temporary
class DocumentSyncManager; // TODO: temporary
class ModelCodecManager; // TODO: temporary

class DocumentManagerPrivate;

class KASTENCORE_EXPORT DocumentManager : public QObject
{
    Q_OBJECT

    friend class DocumentSyncManager;

public:
    DocumentManager();
    ~DocumentManager() override;

public:
    void addDocument(AbstractDocument* document);

    void closeDocument(AbstractDocument* document);
    void closeDocuments(const QVector<AbstractDocument*>& documents);
    void closeAll();
// TODO: think about if a more general close( documentList, theseOrOthers ) is better, same with canCloseAllOther()
    void closeAllOther(AbstractDocument* document);

    // TODO: what to do for documents not added?
    bool canClose(AbstractDocument* document) const;
    bool canClose(const QVector<AbstractDocument*>& documents) const;
    bool canCloseAll() const;
    bool canCloseAllOther(AbstractDocument* document) const;

    void requestFocus(AbstractDocument* document);

public:
    QVector<AbstractDocument*> documents() const;
    bool isEmpty() const;

public:
    DocumentCreateManager* createManager() const;
    DocumentSyncManager* syncManager() const;
    ModelCodecManager* codecManager() const;

Q_SIGNALS:
    // documents got added
    void added(const QVector<Kasten::AbstractDocument*>& documents);
    /// documents are about to be closed, cannot be stopped
    void closing(const QVector<Kasten::AbstractDocument*>& documents);

//     void closing( KCloseEvent* event );
// TODO: other than QObject event gets modified by observers, take care of unsetting a close cancel
// problem with a signal is that all(!) observers get notified, even if event is already cancelled
// better a visitor pattern?

    // TODO: or should the document be able to emit this?
    void focusRequested(Kasten::AbstractDocument* document);

private:
    const QScopedPointer<class DocumentManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DocumentManager)
};

}

#endif
