/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoombarcontroller.hpp"

// lib
#include "zoomslider.hpp"
// Kasten gui
#include <Kasten/StatusBar>

namespace Kasten {

ZoomBarController::ZoomBarController(StatusBar* statusBar)
{
    mZoomSlider = new ZoomSlider(statusBar);
    statusBar->addWidget(mZoomSlider);

    setTargetModel(nullptr);
}

void ZoomBarController::setTargetModel(AbstractModel* model)
{
    mZoomSlider->setTargetModel(model);
}

}
