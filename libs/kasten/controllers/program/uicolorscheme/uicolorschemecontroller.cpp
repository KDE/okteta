/*
    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uicolorschemecontroller.hpp"

// KF
#include <kconfigwidgets_version.h>
#if KCONFIGWIDGETS_VERSION >= QT_VERSION_CHECK(5, 107, 0)
#include <KColorSchemeMenu>
#endif
#include <KColorSchemeManager>
#include <KActionMenu>
#include <KXmlGuiWindow>
#include <KActionCollection>

namespace Kasten {

UiColorSchemeController::UiColorSchemeController(KXmlGuiWindow* window)
{
    auto* const manager = new KColorSchemeManager(this);

#if KCONFIGWIDGETS_VERSION >= QT_VERSION_CHECK(5, 107, 0)
    KActionMenu* const selectionMenu = KColorSchemeMenu::createMenu(manager, this);
#else
    KActionMenu* const selectionMenu = manager->createSchemeSelectionMenu(this);
#endif

    window->actionCollection()->addAction(QStringLiteral("settings_uicolorscheme"), selectionMenu);
}

void UiColorSchemeController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

}

#include "moc_uicolorschemecontroller.cpp"
