/*
    SPDX-FileCopyrightText: 2010-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "terminaltool.hpp"

// Kasten core
#include <Kasten/DocumentSyncManager>
#include <Kasten/AbstractDocument>

// KF
#include <KLocalizedString>
#include <KIO/Global>
// Qt
#include <QUrl>

namespace Kasten {

TerminalTool::TerminalTool(DocumentSyncManager* documentSyncManager)
    : mDocumentSyncManager(documentSyncManager)
{
    setObjectName(QStringLiteral("Terminal"));
}

TerminalTool::~TerminalTool() = default;

QString TerminalTool::title() const { return i18nc("@title:window", "Terminal"); }

QUrl TerminalTool::currentUrl() const
{
    QUrl result;

    if (mDocument) {
        result = KIO::upUrl(mDocumentSyncManager->urlOf(mDocument));
    }

    return result;
}

void TerminalTool::setTargetModel(AbstractModel* model)
{
    AbstractDocument* const document = model ? model->findBaseModel<AbstractDocument*>() : nullptr;
    if (document == mDocument) {
        return;
    }

    const QUrl oldCurrentUrl = currentUrl();

    mDocument = document;

    const QUrl newCurrentUrl = currentUrl();

    if (oldCurrentUrl != newCurrentUrl) {
        Q_EMIT currentUrlChanged(newCurrentUrl);
    }
}

}

#include "moc_terminaltool.cpp"
