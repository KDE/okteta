/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ZOOMCONTROLLER_HPP
#define KASTEN_ZOOMCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/ZoomLevelsQueryable>

class KXMLGUIClient;
class QAction;

namespace Kasten {

namespace If {
class Zoomable;
}

class ZoomController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ZoomController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void zoomIn();
    void zoomOut();
    void zoomNormal();

private Q_SLOTS:
    void updateActionsToZoomScale(double zoomScale);
    void onZoomLevelsChanged();

private:
    AbstractModel* mModel = nullptr;
    If::Zoomable* mZoomControl = nullptr;
    If::ZoomLevelsQueryable* m_zoomLevelsControl = nullptr;

    QAction* mZoomInAction;
    QAction* mZoomOutAction;
    QAction* mZoomNormalAction;
};

}

#endif
