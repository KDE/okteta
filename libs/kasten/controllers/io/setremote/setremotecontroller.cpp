/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "setremotecontroller.hpp"

// Kasten core
#include <Kasten/DocumentSyncManager>
#include <Kasten/AbstractDocument>
// KF
#include <KActionCollection>
#include <KStandardAction>
#include <KXMLGUIClient>
// Qt
#include <QAction>

namespace Kasten {

SetRemoteController::SetRemoteController(DocumentSyncManager* syncManager, KXMLGUIClient* guiClient)
    : mSyncManager(syncManager)
{
    mSaveAsAction = KStandardAction::saveAs(this, &SetRemoteController::saveAs, this);
    mSaveAsAction->setEnabled(false);

    guiClient->actionCollection()->addAction(mSaveAsAction->objectName(), mSaveAsAction);
}

void SetRemoteController::setTargetModel(AbstractModel* model)
{
    AbstractDocument* const document = model ? model->findBaseModel<AbstractDocument*>() : nullptr;
    if (document == mDocument) {
        return;
    }

    mDocument = document;

    const bool canBeSaved = mDocument ?
                            (mDocument->synchronizer() ||
                             mSyncManager->hasSynchronizerForLocal(mDocument->mimeType())) :
                            false;

    mSaveAsAction->setEnabled(canBeSaved);
}

void SetRemoteController::saveAs()
{
    std::ignore = mSyncManager->setSynchronizer(mDocument);
}

}

#include "moc_setremotecontroller.cpp"
