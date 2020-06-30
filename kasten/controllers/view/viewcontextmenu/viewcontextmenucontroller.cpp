/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewcontextmenucontroller.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// KF
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KLocalizedString>
// Qt
#include <QMenu>

namespace Kasten {

ViewContextMenuController::ViewContextMenuController(KXMLGUIClient* guiClient)
    : mGuiClient(guiClient)
{
    setTargetModel(nullptr);
}

void ViewContextMenuController::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    if (mByteArrayView) {
        connect(mByteArrayView, &ByteArrayView::viewContextMenuRequested,
                this, &ViewContextMenuController::showContextMenu);
    }
}

void ViewContextMenuController::showContextMenu(QPoint pos)
{
    QWidget* w = mGuiClient->factory()->container(QStringLiteral("viewContextMenu"), mGuiClient);
    auto* popup = static_cast<QMenu*>(w);

    const auto popupPoint = mByteArrayView->widget()->mapToGlobal(pos);

    popup->popup(popupPoint);
}

}
