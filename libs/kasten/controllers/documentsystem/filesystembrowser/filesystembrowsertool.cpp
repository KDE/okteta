/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filesystembrowsertool.hpp"

// Kasten core
#include <Kasten/DocumentSyncManager>
#include <Kasten/Versionable>
#include <Kasten/AbstractDocument>
#include <Kasten/AbstractModelSynchronizer>

// KF
#include <KLocalizedString>
#include <KIO/Global>

#include <QUrl>

namespace Kasten {

FileSystemBrowserTool::FileSystemBrowserTool(DocumentSyncManager* documentSyncManager)
    : mDocumentSyncManager(documentSyncManager)
{
    setObjectName(QStringLiteral("FileSystemBrowser"));
}

FileSystemBrowserTool::~FileSystemBrowserTool() = default;

QString FileSystemBrowserTool::title() const { return i18nc("@title:window", "Filesystem"); }

QUrl FileSystemBrowserTool::currentUrl() const
{
    QUrl result;

    if (mDocument) {
        result = KIO::upUrl(mDocumentSyncManager->urlOf(mDocument));
    }

    return result;
}

bool FileSystemBrowserTool::hasCurrentUrl() const
{
    return (mDocument && mDocument->synchronizer());
}

void FileSystemBrowserTool::setTargetModel(AbstractModel* model)
{
    const bool oldHasCurrentUrl = hasCurrentUrl();

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;

    const bool newHasCurrentUrl = hasCurrentUrl();

    if (oldHasCurrentUrl != newHasCurrentUrl) {
        emit hasCurrentUrlChanged(newHasCurrentUrl);
    }
}

void FileSystemBrowserTool::open(const QUrl& url)
{
    mDocumentSyncManager->load(url);
}

}
