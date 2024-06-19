/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTMANAGER_P_HPP
#define KASTEN_DOCUMENTMANAGER_P_HPP

// lib
#include "documentmanager.hpp"
// Qt
#include <QVector>
// Std
#include <memory>

namespace Kasten {

class AbstractDocument;

class DocumentCreateManager; // TODO: temporary
class DocumentSyncManager; // TODO: temporary
class ModelCodecManager; // TODO: temporary

class DocumentManagerPrivate
{
public:
    explicit DocumentManagerPrivate(DocumentManager* q);
    DocumentManagerPrivate(const DocumentManagerPrivate&) = delete;

    ~DocumentManagerPrivate();

    DocumentManagerPrivate& operator=(const DocumentManagerPrivate&) = delete;

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

private:
    DocumentManager* const q_ptr;

    QVector<AbstractDocument*> mList;

    // TODO: remove into own singleton
    const std::unique_ptr<DocumentCreateManager> mCreateManager;
    const std::unique_ptr<DocumentSyncManager> mSyncManager;
    const std::unique_ptr<ModelCodecManager> mCodecManager;

    Q_DECLARE_PUBLIC(DocumentManager)
};


inline DocumentCreateManager* DocumentManagerPrivate::createManager() const { return mCreateManager.get(); }
inline DocumentSyncManager* DocumentManagerPrivate::syncManager() const { return mSyncManager.get(); }
inline ModelCodecManager* DocumentManagerPrivate::codecManager() const { return mCodecManager.get(); }

inline QVector<AbstractDocument*> DocumentManagerPrivate::documents() const { return mList; }
inline bool DocumentManagerPrivate::isEmpty() const { return mList.isEmpty(); }

}

#endif
