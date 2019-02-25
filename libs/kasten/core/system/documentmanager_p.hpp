/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006-2007,2009,2011,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_DOCUMENTMANAGER_P_HPP
#define KASTEN_DOCUMENTMANAGER_P_HPP

// lib
#include "documentmanager.hpp"
// Qt
#include <QVector>

class QStringList;

namespace Kasten {

class AbstractDocument;

class DocumentCreateManager; // TODO: temporary
class DocumentSyncManager; // TODO: temporary
class ModelCodecManager; // TODO: temporary

class DocumentManagerPrivate
{
public:
    explicit DocumentManagerPrivate(DocumentManager* q);
    ~DocumentManagerPrivate();

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
    DocumentCreateManager* mCreateManager;
    DocumentSyncManager* mSyncManager;
    ModelCodecManager* mCodecManager;

    Q_DECLARE_PUBLIC(DocumentManager)
};


inline DocumentCreateManager* DocumentManagerPrivate::createManager() const { return mCreateManager; }
inline DocumentSyncManager* DocumentManagerPrivate::syncManager() const { return mSyncManager; }
inline ModelCodecManager* DocumentManagerPrivate::codecManager() const { return mCodecManager; }

inline QVector<AbstractDocument*> DocumentManagerPrivate::documents() const { return mList; }
inline bool DocumentManagerPrivate::isEmpty() const { return mList.isEmpty(); }

}

#endif
