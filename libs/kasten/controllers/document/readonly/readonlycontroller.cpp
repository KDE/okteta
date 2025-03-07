/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "readonlycontroller.hpp"

// Kasten core
#include <Kasten/AbstractDocument>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KToggleAction>
#include <KGuiItem>

namespace Kasten {

ReadOnlyController::ReadOnlyController(KXMLGUIClient* guiClient)
{
    mSetReadOnlyAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("object-unlocked")),
                                           i18nc("@option:check set the document to read-only", "Set Read-only"),
                                           this);
    const QString checkedText = i18nc("@option:check set the document to read-write", "Set Read-write");
    const KGuiItem checkedState(checkedText, QIcon::fromTheme(QStringLiteral("object-locked")));
    mSetReadOnlyAction->setCheckedState(checkedState);
    connect(mSetReadOnlyAction, &QAction::triggered,
            this, &ReadOnlyController::setReadOnly);
    mSetReadOnlyAction->setEnabled(false);

    guiClient->actionCollection()->addAction(QStringLiteral("isreadonly"), mSetReadOnlyAction);
}

void ReadOnlyController::setTargetModel(AbstractModel* model)
{
    AbstractDocument* const document = model ? model->findBaseModel<AbstractDocument*>() : nullptr;
    if (document == mDocument) {
        return;
    }

    if (mDocument) {
        mDocument->disconnect(mSetReadOnlyAction);
    }

    mDocument = document;

    if (mDocument) {
        mSetReadOnlyAction->setChecked(mDocument->isReadOnly());

        connect(mDocument, &AbstractModel::readOnlyChanged,
                mSetReadOnlyAction, &QAction::setChecked);
        connect(mDocument, &AbstractModel::modifiableChanged,
                mSetReadOnlyAction, &QAction::setEnabled);
    }

    mSetReadOnlyAction->setEnabled(mDocument ? mDocument->isModifiable() : false);
}

void ReadOnlyController::setReadOnly(bool isReadOnly)
{
    mDocument->setReadOnly(isReadOnly);
}

}

#include "moc_readonlycontroller.cpp"
