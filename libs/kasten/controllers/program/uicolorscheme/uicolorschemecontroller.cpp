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
#if KCOLORSCHEME_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    auto* manager = KColorSchemeManager::instance();
#else
    auto* manager = new KColorSchemeManager(this);
#endif

    KActionMenu* selectionMenu = KColorSchemeMenu::createMenu(manager, this);

    window->actionCollection()->addAction(QStringLiteral("settings_uicolorscheme"), selectionMenu);
}

void UiColorSchemeController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

}

#include "moc_uicolorschemecontroller.cpp"
