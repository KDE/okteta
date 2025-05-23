/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ZOOMBARCONTROLLER_HPP
#define KASTEN_ZOOMBARCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

namespace Kasten {

class ZoomSlider;
class StatusBar;

class ZoomBarController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ZoomBarController(StatusBar* statusBar);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private:
    ZoomSlider* mZoomSlider;
};

}

#endif
