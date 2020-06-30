/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILESYSTEMBROWSERTOOL_HPP
#define KASTEN_FILESYSTEMBROWSERTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>

class QUrl;

namespace Kasten {

class DocumentSyncManager;
class AbstractDocument;

class FileSystemBrowserTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit FileSystemBrowserTool(DocumentSyncManager* documentSyncManager);
    ~FileSystemBrowserTool() override;

public:
    void open(const QUrl& url);

public:
    QUrl currentUrl() const;
    bool hasCurrentUrl() const;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

Q_SIGNALS:
    void hasCurrentUrlChanged(bool hasCurrentUrl);

private: // sources
    DocumentSyncManager* mDocumentSyncManager;

    AbstractDocument* mDocument = nullptr;
};

}

#endif
