/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoomcontroller.hpp"

// Kasten gui
#include <Kasten/Zoomable>
// Kasten core
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QAction>

namespace Kasten {

ZoomController::ZoomController(KXMLGUIClient* guiClient)
{
    mZoomInAction = KStandardAction::zoomIn(  this, &ZoomController::zoomIn,  this);
    mZoomOutAction = KStandardAction::zoomOut(this, &ZoomController::zoomOut, this);

    guiClient->actionCollection()->addActions({
        mZoomInAction,
        mZoomOutAction
    });

#if 0
    ZoomToAction = new KSelectAction(i18n("Zoom"), "viewmag", 0, ActionCollection, "zoomTo");
    ZoomToAction->setEditable(true);
    QList<double> mags = DisplayOptions::normalMagnificationValues();
    QStringList translated;
    int idx = 0;
    int cur = 0;
    for (QList<double>::iterator first = mags.begin(), last = mags.end();
         first != last;
         ++first) {
        translated << i18nc("zoom-factor (percentage)", "%1%", *first * 100.0);
        if (*first == 1.0) {
            idx = cur;
        }
        ++cur;
    }

    ZoomToAction->setItems(translated);
    ZoomToAction->setCurrentItem(idx);
    connect(ZoomToAction, SIGNAL(triggered(QString)), SLOT(zoomTo(QString)));

    // TODO: name size relative to object or view? name object(variable) or view?
    // TODO: is this a sticking parameter?
    FitToWidthAction = new KAction(i18n("&Fit to Width"), ActionCollection, "fit_to_width");
    connect(FitWidthAction, SIGNAL(triggered(bool)), SLOT(fitToWidth()));
    FitToHeightAction = new KAction(i18n("&Fit to Height"), ActionCollection, "fit_to_height");
    connect(FitWidthAction, SIGNAL(triggered(bool)), SLOT(fitToHeight()));
    FitToSizeAction = new KAction(i18n("&Fit to Size"), ActionCollection, "fit_to_size");
    connect(FitToSizeAction, SIGNAL(triggered(bool)), SLOT(fitToSize()));
#endif
    setTargetModel(nullptr);
}

void ZoomController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::Zoomable*>() : nullptr;
    mZoomControl = mModel ? qobject_cast<If::Zoomable*>(mModel) : nullptr;

    if (mZoomControl) {
        mZoomLevel = mZoomControl->zoomLevel();
        connect(mModel, SIGNAL(zoomLevelChanged(double)), SLOT(onZoomLevelChange(double)));
    }

    const bool hasView = (mZoomControl != nullptr);
    mZoomInAction->setEnabled(hasView);
    mZoomOutAction->setEnabled(hasView);
}

void ZoomController::zoomIn()
{
    mZoomControl->setZoomLevel(mZoomLevel * 1.10);
}

void ZoomController::zoomOut()
{
    mZoomControl->setZoomLevel(mZoomLevel / 1.10);
}
#if 0
void ZoomController::zoomTo(const QString& nz)
{
    QString z = nz;
    double zoom;
    z.remove(z.indexOf('%'), 1);
    zoom = KLocale::global()->readNumber(z) / 100;
    qCDebug(LOG_KASTEN_CONTROLLERS) << "ZOOM = "  << nz << ", setting zoom = " << zoom << endl;

    DisplayOptions options = miniWidget()->displayOptions();
    options.setMagnification(zoom);
    miniWidget()->setDisplayOptions(options);
    miniWidget()->redisplay();
    _mainWidget->setFocus();
    updateZoomActions();
}

void ZoomController::fitToWidth()
{
    if (pageView()->page()) {
        miniWidget()->fitWidth(pageView()->viewport()->width() - 16);
    }
    // We subtract 16 pixels because of the page decoration.
    updateZoomActions();
}

void ZoomController::fitToSize()
{
    if (pageView()->page()) {
        miniWidget()->fitWidthHeight(pageView()->viewport()->width() - 16,
                                     pageView()->viewport()->height() - 16);
    }
    updateZoomActions();
}
#endif
void ZoomController::onZoomLevelChange(double level)
{
    mZoomLevel = level;
}

}
