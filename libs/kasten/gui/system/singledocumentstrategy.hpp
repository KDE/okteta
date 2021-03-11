/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINGLEDOCUMENTSTRATEGY_HPP
#define KASTEN_SINGLEDOCUMENTSTRATEGY_HPP

// lib
#include "abstractdocumentstrategy.hpp"

namespace Kasten {
class ViewManager;
class DocumentManager;
class SingleDocumentStrategyPrivate;

class KASTENGUI_EXPORT SingleDocumentStrategy : public AbstractDocumentStrategy
{
    Q_OBJECT

public:
    SingleDocumentStrategy(DocumentManager* documentManager,
                           ViewManager* viewManager);
    ~SingleDocumentStrategy() override;

public: // AbstractDocumentStrategy API
    void createNew() override;
    void createNewFromClipboard() override;
    void createNewWithGenerator(AbstractModelDataGenerator* generator) override;

    void load(const QUrl& url) override;

    void closeDocument(AbstractDocument* document) override;
    void closeAll() override;
    void closeAllOther(AbstractDocument* document) override;

public: // const AbstractDocumentStrategy API
    QVector<AbstractDocument*> documents() const override;
    QStringList supportedRemoteTypes() const override;

    bool canClose(AbstractDocument* document) const override;
    bool canCloseAll() const override;
    bool canCloseAllOther(AbstractDocument* document) const override;

private:
    Q_DECLARE_PRIVATE(SingleDocumentStrategy)
};

}

#endif
