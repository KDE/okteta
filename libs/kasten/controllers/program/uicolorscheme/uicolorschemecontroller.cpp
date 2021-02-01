/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uicolorschemecontroller.hpp"

// KF
#include <KColorSchemeManager>
#include <KActionMenu>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KXmlGuiWindow>
#include <KActionCollection>
// Qt
#include <QMenu>
#include <QModelIndex>

namespace Kasten {

static constexpr char UiSettingsConfigGroupId[] = "UiSettings";
static constexpr char ColorSchemeConfigKeyId[] = "ColorScheme";

UiColorSchemeController::UiColorSchemeController(KXmlGuiWindow* window)
{
    auto* manager = new KColorSchemeManager(this);

    KConfigGroup configGroup(KSharedConfig::openConfig(), UiSettingsConfigGroupId);
    // empty string: system default
    const QString schemeName = configGroup.readEntry(ColorSchemeConfigKeyId);

    KActionMenu* selectionMenu = manager->createSchemeSelectionMenu(schemeName, this);
    QMenu* menu = selectionMenu->menu();

    connect(menu, &QMenu::triggered,
            this, &UiColorSchemeController::handleSchemeChanged);

    manager->activateScheme(manager->indexForScheme(schemeName));

    window->actionCollection()->addAction(QStringLiteral("settings_uicolorscheme"), selectionMenu);
}

void UiColorSchemeController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void UiColorSchemeController::handleSchemeChanged(QAction* action)
{
    const QString schemeName = KLocalizedString::removeAcceleratorMarker(action->text());

    KConfigGroup configGroup(KSharedConfig::openConfig(), UiSettingsConfigGroupId);
    configGroup.writeEntry(ColorSchemeConfigKeyId, schemeName);
    configGroup.sync();
}

}
