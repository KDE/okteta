/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSTOOL_HPP
#define KASTEN_DOCUMENTSTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>

namespace Kasten {

class DocumentManager;
class AbstractDocument;

class DocumentsTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit DocumentsTool(DocumentManager* documentManager);
    ~DocumentsTool() override;

public:
    AbstractDocument* focussedDocument() const;
    QVector<AbstractDocument*> documents() const;

public:
    void setFocussedDocument(AbstractDocument* document);

public: // AbstractTool API
    QString title() const override;
    void setTargetModel(AbstractModel* model) override;

Q_SIGNALS:
    void documentsAdded(const QVector<Kasten::AbstractDocument*>& documents);
    void documentsClosing(const QVector<Kasten::AbstractDocument*>& documents);
    void focussedDocumentChanged(Kasten::AbstractDocument* document);

private: // sources
    DocumentManager* mDocumentManager;

    AbstractDocument* mFocussedDocument = nullptr;
};

inline AbstractDocument* DocumentsTool::focussedDocument() const { return mFocussedDocument; }

}

#endif
