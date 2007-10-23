/***************************************************************************
                          zoomcontroller.h  -  description
                             -------------------
    begin                : Wed Nov 8 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef ZOOMCONTROLLER_H
#define ZOOMCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KAction;
class KViewManager;
namespace KDE { namespace If {
class Zoomable;
} }


class ZoomController : public KViewController
{
  Q_OBJECT

  public:
    ZoomController( KXmlGuiWindow *MW );

  public: // KToolet API
    virtual void setView( KAbstractView *View );

  protected Q_SLOTS: // action slots
    void zoomIn();
    void zoomOut();

  private Q_SLOTS:
    void onZoomLevelChange( double Level );

  protected:
    KXmlGuiWindow *MainWindow;
    QObject *ZoomObject;
    KDE::If::Zoomable *ZoomControl;

    double ZoomLevel;

    KAction *ZoomInAction;
    KAction *ZoomOutAction;
};

#endif
