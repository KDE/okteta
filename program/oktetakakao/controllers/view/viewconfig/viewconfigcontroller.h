/***************************************************************************
                          viewconfigcontroller.h  -  description
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


#ifndef VIEWCONFIGCONTROLLER_H
#define VIEWCONFIGCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KSelectAction;
class KToggleAction;
class KViewManager;
namespace KHEUI {
class KByteArrayView;
}


class ViewConfigController : public KViewController
{
  Q_OBJECT

  public:
    ViewConfigController( KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void setCoding( int Coding );
    void setEncoding( int Encoding );
    void setShowsNonprinting( bool on );
    void setResizeStyle( int Style );
    void toggleOffsetColumn( bool on );
    void toggleValueCharColumns( int visibleColunms );

  protected:
    KXmlGuiWindow *MainWindow;
    KHEUI::KByteArrayView *ViewWidget;

    // view menu
    KSelectAction *CodingAction;
    KSelectAction *EncodingAction;
    KToggleAction *ShowsNonprintingAction;
    // settings menu
    KSelectAction *ResizeStyleAction;
    KToggleAction *ShowOffsetColumnAction;
    KSelectAction *ToggleColumnsAction;
};

#endif
