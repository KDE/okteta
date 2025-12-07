/*
    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uicolorschemecontroller.hpp"

// KF
#include <KColorSchemeMenu>
#include <KColorSchemeManager>
#include <KActionMenu>
#include <KXmlGuiWindow>
#include <KActionCollection>

namespace Kasten {

UiColorSchemeController::UiColorSchemeController(KXmlGuiWindow* window)
{
    auto* const manager = KColorSchemeManager::instance();

    KActionMenu* const selectionMenu = KColorSchemeMenu::createMenu(manager, this);

    window->actionCollection()->addAction(QStringLiteral("settings_uicolorscheme"), selectionMenu);
}

void UiColorSchemeController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

}

#include "moc_uicolorschemecontroller.cpp"
