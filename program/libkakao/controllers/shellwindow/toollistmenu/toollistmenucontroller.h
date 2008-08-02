/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef TOOLLISTMENUCONTROLLER_H
#define TOOLLISTMENUCONTROLLER_H


// lib
#include <kviewcontroller.h>
// Qt
#include <QtCore/QList>

class QAction;
class KXMLGUIClient;
class KViewManager;

namespace KDE { namespace If {
class WidgetsDockable;
}}

class ToolListMenuController : public KViewController
{
  Q_OBJECT

  public:
    ToolListMenuController( KDE::If::WidgetsDockable* widgetsDockable, KXMLGUIClient* guiClient );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  private:
    void updateActions();

  protected:
    KDE::If::WidgetsDockable *mWidgetsDockable;
    KXMLGUIClient* mGuiClient;

    QList<QAction*> mToolActionList;
};

#endif
