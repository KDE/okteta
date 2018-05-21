/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "closecontroller.h"

// Kasten gui
#include <kasten/abstractdocumentstrategy.h>
// Kasten core
#include <kasten/abstractdocument.h>
// KF5
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
    : AbstractXmlGuiController()
    , mDocumentStrategy(documentStrategy)
    , mDocument(nullptr)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    const QIcon documentCloseIcon = QIcon::fromTheme(QStringLiteral("document-close"));
    mCloseAction  = KStandardAction::close(this, &CloseController::close, actionCollection);
    mCloseAction->setEnabled(false);

    if (supportMultiple) {
        mCloseAllAction = actionCollection->addAction(QStringLiteral("file_close_all"),
                                                      this, &CloseController::closeAll);
        mCloseAllAction->setText(i18nc("@title:menu", "Close All"));
        mCloseAllAction->setIcon(documentCloseIcon);
        mCloseAllAction->setEnabled(false);

        mCloseAllOtherAction = actionCollection->addAction(QStringLiteral("file_close_all_other"),
                                                           this, &CloseController::closeAllOther);
        mCloseAllOtherAction->setText(i18nc("@title:menu", "Close All Other"));
        mCloseAllOtherAction->setIcon(documentCloseIcon);
        mCloseAllOtherAction->setEnabled(false);

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
    const QList<AbstractDocument*> documents = mDocumentStrategy->documents();

    const bool hasDocuments = !documents.isEmpty();
    // TODO: there could be just one, but not set for this tool?
    const bool hasOtherDocuments = (documents.size() > 1);

    mCloseAllAction->setEnabled(hasDocuments);
    mCloseAllOtherAction->setEnabled(hasOtherDocuments);
}

}
