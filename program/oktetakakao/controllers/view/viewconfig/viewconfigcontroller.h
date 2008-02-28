/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VIEWCONFIGCONTROLLER_H
#define VIEWCONFIGCONTROLLER_H

// Kakao gui
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
