/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentstool.hpp"

// Kasten core
#include <Kasten/DocumentManager>
#include <Kasten/AbstractDocument>
// KF
#include <KLocalizedString>

namespace Kasten {

DocumentsTool::DocumentsTool(DocumentManager* documentManager)
    : mDocumentManager(documentManager)
{
    setObjectName(QStringLiteral("Documents"));

    connect(mDocumentManager, &DocumentManager::added,
            this, &DocumentsTool::documentsAdded);
    connect(mDocumentManager, &DocumentManager::closing,
            this, &DocumentsTool::documentsClosing);
}

DocumentsTool::~DocumentsTool() = default;

QVector<AbstractDocument*> DocumentsTool::documents() const { return mDocumentManager->documents(); }

QString DocumentsTool::title() const { return i18nc("@title:window", "Documents"); }

// TODO: this is an abuse of setTargetModel. Find other way to get focused model
void DocumentsTool::setTargetModel(AbstractModel* model)
{
    mFocussedDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;
    emit focussedDocumentChanged(mFocussedDocument);
}

void DocumentsTool::setFocussedDocument(AbstractDocument* document)
{
    mDocumentManager->requestFocus(document);
}

}
