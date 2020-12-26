/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewmodecontroller.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// KF
#include <kwidgetsaddons_version.h>
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KSelectAction>

namespace Kasten {

ViewModeController::ViewModeController(KXMLGUIClient* guiClient)
{
    // view style
    mViewModeAction = new KSelectAction(i18nc("@title:menu", "&View Mode"), this);
    mViewModeAction->setItems(QStringList {
        i18nc("@item:inmenu", "&Columns"),
        i18nc("@item:inmenu", "&Rows"),
    });
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 78, 0)
    connect(mViewModeAction, &KSelectAction::indexTriggered,
#else
    connect(mViewModeAction, QOverload<int>::of(&KSelectAction::triggered),
#endif
            this, &ViewModeController::setViewMode);

    guiClient->actionCollection()->addAction(QStringLiteral("viewmode"), mViewModeAction);

    setTargetModel(nullptr);
}

void ViewModeController::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    const bool hasView = (mByteArrayView != nullptr);
    if (hasView) {
        onViewModusChanged((int)mByteArrayView->viewModus());
        connect(mByteArrayView, &ByteArrayView::viewModusChanged,
                this, &ViewModeController::onViewModusChanged);

    }
    mViewModeAction->setEnabled(hasView);
}

void ViewModeController::setViewMode(int viewMode)
{
    mByteArrayView->setViewModus(viewMode);
}

void ViewModeController::onViewModusChanged(int viewModus)
{
    mViewModeAction->setCurrentItem(viewModus);
}

}
