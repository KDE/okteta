/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINGLEDOCUMENTSTRATEGY_P_HPP
#define KASTEN_SINGLEDOCUMENTSTRATEGY_P_HPP

// lib
#include "abstractdocumentstrategy_p.hpp"
#include "singledocumentstrategy.hpp"
// Kasten core
#include <Kasten/DocumentManager>
#include <Kasten/DocumentSyncManager>
// Qt Core
#include <QStringList>

namespace Kasten {
class ViewManager;

class SingleDocumentStrategyPrivate : public AbstractDocumentStrategyPrivate
{
public:
    SingleDocumentStrategyPrivate(SingleDocumentStrategy* parent,
                                  DocumentManager* documentManager,
                                  ViewManager* viewManager);
    ~SingleDocumentStrategyPrivate() override;

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
    Q_DECLARE_PUBLIC(SingleDocumentStrategy)

private:
    DocumentManager* mDocumentManager;
    ViewManager* mViewManager;
};

inline
SingleDocumentStrategyPrivate::SingleDocumentStrategyPrivate(SingleDocumentStrategy* parent,
                                                             DocumentManager* documentManager,
                                                             ViewManager* viewManager)
    : AbstractDocumentStrategyPrivate(parent)
    , mDocumentManager(documentManager)
    , mViewManager(viewManager)
{
}

inline SingleDocumentStrategyPrivate::~SingleDocumentStrategyPrivate() = default;

inline QVector<AbstractDocument*> SingleDocumentStrategyPrivate::documents() const
{
    return mDocumentManager->documents();
}

inline QStringList SingleDocumentStrategyPrivate::supportedRemoteTypes() const
{
    return mDocumentManager->syncManager()->supportedRemoteTypes();
}

inline bool SingleDocumentStrategyPrivate::canClose(AbstractDocument* document) const
{
    return mDocumentManager->canClose(document);
}

inline bool SingleDocumentStrategyPrivate::canCloseAllOther(AbstractDocument* document) const
{
    return mDocumentManager->canCloseAllOther(document);
}

inline bool SingleDocumentStrategyPrivate::canCloseAll() const
{
    return mDocumentManager->canCloseAll();
}

inline void SingleDocumentStrategyPrivate::closeAll()
{
    mDocumentManager->closeAll();
}

inline void SingleDocumentStrategyPrivate::closeAllOther(AbstractDocument* document)
{
    mDocumentManager->closeAllOther(document);
}

inline void SingleDocumentStrategyPrivate::closeDocument(AbstractDocument* document)
{
    mDocumentManager->closeDocument(document);
}

}

#endif
