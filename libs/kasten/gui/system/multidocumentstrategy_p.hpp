/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MULTIDOCUMENTSTRATEGY_P_HPP
#define KASTEN_MULTIDOCUMENTSTRATEGY_P_HPP

// lib
#include "abstractdocumentstrategy_p.hpp"
#include "multidocumentstrategy.hpp"
// Kasten core
#include <Kasten/DocumentManager>
#include <Kasten/DocumentSyncManager>
#include <Kasten/DocumentCreateManager>
// Qt Core
#include <QStringList>

namespace Kasten {
class ViewManager;
class MultiDocumentStrategy;

class MultiDocumentStrategyPrivate : public AbstractDocumentStrategyPrivate
{
public:
    MultiDocumentStrategyPrivate(MultiDocumentStrategy* parent,
                                 DocumentManager* documentManager,
                                 ViewManager* viewManager);
    ~MultiDocumentStrategyPrivate() override;

public:
    void init();

public: // AbstractDocumentStrategy API
    void createNew();
    void createNewFromClipboard();
    void createNewWithGenerator(AbstractModelDataGenerator* generator);

    void load(const QUrl& url);

    void closeDocument(AbstractDocument* document);
    void closeAll();
    void closeAllOther(AbstractDocument* document);

public: // const AbstractDocumentStrategy API
    QVector<AbstractDocument*> documents() const;
    QStringList supportedRemoteTypes() const;

    bool canClose(AbstractDocument* document) const;
    bool canCloseAll() const;
    bool canCloseAllOther(AbstractDocument* document) const;

private:
    void triggerGeneration(AbstractModelDataGenerator* generator);

private:
    Q_DECLARE_PUBLIC(MultiDocumentStrategy)

private:
    DocumentManager* mDocumentManager;
    ViewManager* mViewManager;
};

inline
MultiDocumentStrategyPrivate::MultiDocumentStrategyPrivate(MultiDocumentStrategy* parent,
                                                           DocumentManager* documentManager,
                                                           ViewManager* viewManager)
    : AbstractDocumentStrategyPrivate(parent)
    , mDocumentManager(documentManager)
    , mViewManager(viewManager)
{
}

inline MultiDocumentStrategyPrivate::~MultiDocumentStrategyPrivate() = default;

inline QVector<AbstractDocument*> MultiDocumentStrategyPrivate::documents() const
{
    return mDocumentManager->documents();
}

inline QStringList MultiDocumentStrategyPrivate::supportedRemoteTypes() const
{
    return mDocumentManager->syncManager()->supportedRemoteTypes();
}

inline bool MultiDocumentStrategyPrivate::canClose(AbstractDocument* document) const
{
    return mDocumentManager->canClose(document);
}

inline bool MultiDocumentStrategyPrivate::canCloseAllOther(AbstractDocument* document) const
{
    return mDocumentManager->canCloseAllOther(document);
}

inline bool MultiDocumentStrategyPrivate::canCloseAll() const
{
    return mDocumentManager->canCloseAll();
}

inline void MultiDocumentStrategyPrivate::createNew()
{
    mDocumentManager->createManager()->createNew();
}

inline void MultiDocumentStrategyPrivate::load(const QUrl& url)
{
    mDocumentManager->syncManager()->load(url);
}

inline void MultiDocumentStrategyPrivate::closeAll()
{
    mDocumentManager->closeAll();
}

inline void MultiDocumentStrategyPrivate::closeAllOther(AbstractDocument* document)
{
    mDocumentManager->closeAllOther(document);
}

inline void MultiDocumentStrategyPrivate::closeDocument(AbstractDocument* document)
{
    mDocumentManager->closeDocument(document);
}

}

#endif
