/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uicolorschemecontroller.hpp"

// KF
#if KCONFIGWIDGETS_VERSION >= QT_VERSION_CHECK(5, 107, 0)
#include <KColorSchemeMenu>
#endif
#include <KColorSchemeManager>
#include <KActionMenu>
#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 89, 0)
#include <KLocalizedString>
#endif
#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 93, 0)
#include <KSharedConfig>
#include <KConfigGroup>
#endif
#include <KXmlGuiWindow>
#include <KActionCollection>
// Qt
#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 89, 0)
#include <QMenu>
#endif
#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 93, 0)
#include <QModelIndex>
#endif

namespace Kasten {

#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 93, 0)
static constexpr char UiSettingsConfigGroupId[] = "UiSettings";
static constexpr char ColorSchemeConfigKeyId[] = "ColorScheme";
#endif

UiColorSchemeController::UiColorSchemeController(KXmlGuiWindow* window)
{
    auto* manager = new KColorSchemeManager(this);

#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 93, 0)
    KConfigGroup configGroup(KSharedConfig::openConfig(), UiSettingsConfigGroupId);
    // empty string: system default
    const QString schemeName = configGroup.readEntry(ColorSchemeConfigKeyId);

    KActionMenu* selectionMenu = manager->createSchemeSelectionMenu(schemeName, this);

#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 89, 0)
    QMenu* menu = selectionMenu->menu();

    connect(menu, &QMenu::triggered,
            this, &UiColorSchemeController::handleSchemeChanged);
#endif

    manager->activateScheme(manager->indexForScheme(schemeName));
#elif KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 107, 0)
    KActionMenu* selectionMenu = manager->createSchemeSelectionMenu(this);
#else
    KActionMenu* selectionMenu = KColorSchemeMenu::createMenu(manager, this);
#endif

    window->actionCollection()->addAction(QStringLiteral("settings_uicolorscheme"), selectionMenu);
}

void UiColorSchemeController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

#if KCONFIGWIDGETS_VERSION < QT_VERSION_CHECK(5, 89, 0)
void UiColorSchemeController::handleSchemeChanged(QAction* action)
{
    const QString schemeName = KLocalizedString::removeAcceleratorMarker(action->text());

    KConfigGroup configGroup(KSharedConfig::openConfig(), UiSettingsConfigGroupId);
    configGroup.writeEntry(ColorSchemeConfigKeyId, schemeName);
    configGroup.sync();
}
#endif

}
