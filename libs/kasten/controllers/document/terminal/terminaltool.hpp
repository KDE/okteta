/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TERMINALTOOL_HPP
#define KASTEN_TERMINALTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>

class QUrl;

namespace Kasten {

class DocumentSyncManager;
class AbstractDocument;

class TerminalTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit TerminalTool(DocumentSyncManager* documentSyncManager);
    ~TerminalTool() override;

public:
    QUrl currentUrl() const;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

Q_SIGNALS:
    void currentUrlChanged(const QUrl& currentUrl);

private: // sources
    DocumentSyncManager* mDocumentSyncManager;

    AbstractDocument* mDocument = nullptr;
};

}

#endif
