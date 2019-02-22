/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewcontextmenucontroller.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// KF5
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

void ViewContextMenuController::showContextMenu(const QPoint& pos)
{
    QWidget* w = mGuiClient->factory()->container(QStringLiteral("viewContextMenu"), mGuiClient);
    auto* popup = static_cast<QMenu*>(w);

    const auto popupPoint = mByteArrayView->widget()->mapToGlobal(pos);

    popup->popup(popupPoint);
}

}
