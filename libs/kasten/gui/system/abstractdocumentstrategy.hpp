/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTDOCUMENTSTRATEGY_HPP
#define KASTEN_ABSTRACTDOCUMENTSTRATEGY_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QObject>

template <typename C> class QVector;
class QUrl;

namespace Kasten {
class AbstractDocument;
class AbstractModelDataGenerator;
class AbstractDocumentStrategyPrivate;

class KASTENGUI_EXPORT AbstractDocumentStrategy : public QObject
{
    Q_OBJECT

protected:
    AbstractDocumentStrategy();
    explicit AbstractDocumentStrategy(AbstractDocumentStrategyPrivate* d);

public:
    ~AbstractDocumentStrategy() override;

public: // API to be implemented
    virtual void createNew() = 0;
    virtual void createNewFromClipboard() = 0;
    virtual void createNewWithGenerator(AbstractModelDataGenerator* generator) = 0;

    virtual void load(const QUrl& url) = 0;

    virtual void closeDocument(AbstractDocument* document) = 0;
//     virtual void closeDocuments( const QVector<AbstractDocument*>& documents ) = 0;
    virtual void closeAll() = 0;
// TODO: think about if a more general close( documentList, theseOrOthers ) is better, same with canCloseAllOther()
    virtual void closeAllOther(AbstractDocument* document) = 0;

//     virtual void requestFocus( AbstractDocument* document ) = 0;

public: // const API to be implemented
    /// Returns \c true if there
//     virtual bool allowsMultipleDocuments() const = 0;
    virtual QVector<AbstractDocument*> documents() const = 0;
    virtual QStringList supportedRemoteTypes() const = 0;

    // TODO: what to do for documents not added?
    virtual bool canClose(AbstractDocument* document) const = 0;
//     virtual bool canClose( const QVector<AbstractDocument*>& documents ) const = 0;
    virtual bool canCloseAll() const = 0;
    virtual bool canCloseAllOther(AbstractDocument* document) const = 0;

Q_SIGNALS:
    // documents got added
    void added(const QVector<Kasten::AbstractDocument*>& documents);
    /// documents are about to be closed, cannot be stopped
    void closing(const QVector<Kasten::AbstractDocument*>& documents);

    // TODO: or should the document be able to emit this?
    void focusRequested(Kasten::AbstractDocument* document);

    void urlUsed(const QUrl& url);

protected:
    const QScopedPointer<AbstractDocumentStrategyPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractDocumentStrategy)
};

}

#endif
