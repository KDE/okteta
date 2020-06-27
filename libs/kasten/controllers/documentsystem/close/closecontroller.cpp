/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "closecontroller.hpp"

// Kasten gui
#include <Kasten/AbstractDocumentStrategy>
// Kasten core
#include <Kasten/AbstractDocument>
// KF
#include <KActionCollection>
#include <KStandardAction>
#include <KXMLGUIClient>
#include <KLocalizedString>
// Qt
#include <QAction>

namespace Kasten {

CloseController::CloseController(AbstractDocumentStrategy* documentStrategy,
                                 KXMLGUIClient* guiClient,
                                 bool supportMultiple)
    : mDocumentStrategy(documentStrategy)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    const QIcon documentCloseIcon = QIcon::fromTheme(QStringLiteral("document-close"));
    mCloseAction  = KStandardAction::close(this, &CloseController::close, this);
    mCloseAction->setEnabled(false);

    actionCollection->addAction(mCloseAction->objectName(), mCloseAction);

    if (supportMultiple) {
        mCloseAllAction = new QAction(documentCloseIcon,
                                      i18nc("@action:inmenu", "Close All"), this);
        mCloseAllAction->setObjectName(QStringLiteral("file_close_all"));
        connect(mCloseAllAction, &QAction::triggered,
                this, &CloseController::closeAll);
        mCloseAllAction->setEnabled(false);

        mCloseAllOtherAction = new QAction(documentCloseIcon,
                                           i18nc("@action:inmenu", "Close All Other"), this);
        mCloseAllOtherAction->setObjectName(QStringLiteral("file_close_all_other"));
        connect(mCloseAllOtherAction, &QAction::triggered,
                this, &CloseController::closeAllOther);
        mCloseAllOtherAction->setEnabled(false);

        actionCollection->addAction(mCloseAllAction->objectName(), mCloseAllAction);
        actionCollection->addAction(mCloseAllOtherAction->objectName(), mCloseAllOtherAction);

        connect(mDocumentStrategy, &Kasten::AbstractDocumentStrategy::added,
                this, &CloseController::onDocumentsChanged);
        connect(mDocumentStrategy, &Kasten::AbstractDocumentStrategy::closing,
                this, &CloseController::onDocumentsChanged);
    }
}

CloseController::~CloseController() = default;

void CloseController::setTargetModel(AbstractModel* model)
{
    mDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;
    const bool hasDocument = (mDocument != nullptr);

    mCloseAction->setEnabled(hasDocument);
}

void CloseController::close()
{
    if (mDocumentStrategy->canClose(mDocument)) {
        mDocumentStrategy->closeDocument(mDocument);
    }
}

void CloseController::closeAll()
{
    if (mDocumentStrategy->canCloseAll()) {
        mDocumentStrategy->closeAll();
    }
}

void CloseController::closeAllOther()
{
    if (mDocumentStrategy->canCloseAllOther(mDocument)) {
        mDocumentStrategy->closeAllOther(mDocument);
    }
}

void CloseController::onDocumentsChanged()
{
    const QVector<AbstractDocument*> documents = mDocumentStrategy->documents();

    const bool hasDocuments = !documents.isEmpty();
    // TODO: there could be just one, but not set for this tool?
    const bool hasOtherDocuments = (documents.size() > 1);

    mCloseAllAction->setEnabled(hasDocuments);
    mCloseAllOtherAction->setEnabled(hasOtherDocuments);
}

}
