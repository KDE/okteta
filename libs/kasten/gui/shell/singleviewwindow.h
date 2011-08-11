/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SINGLEVIEWWINDOW_H
#define SINGLEVIEWWINDOW_H

// lib
#include "kastengui_export.h"
#include "widgetsdockable.h"
// KDE
#include <KXmlGuiWindow>

template<class T> class QList;
class QMimeData;


namespace Kasten
{

class SingleViewWindowPrivate;

class AbstractView;
class DocumentManager;
class AbstractToolView;
class AbstractXmlGuiController;
class SingleViewArea;


class KASTENGUI_EXPORT SingleViewWindow : public KXmlGuiWindow,
                                          public If::WidgetsDockable
{
   Q_OBJECT
   Q_INTERFACES( Kasten::If::WidgetsDockable )

  public:
    explicit SingleViewWindow( DocumentManager* documentManager, AbstractView* view );
    virtual ~SingleViewWindow();

  public:
    void addXmlGuiController( AbstractXmlGuiController* controller );
    void addTool( AbstractToolView* toolView );

    void setView( AbstractView* view );

  public: // If::WidgetsDockable API
    virtual QList<ToolViewDockWidget*> dockWidgets() const;

  protected: // KMainWindow API
    virtual bool queryClose();

  protected:
    AbstractView* view() const;
    SingleViewArea* viewArea() const;
    DocumentManager* documentManager() const;

  private:
    Q_PRIVATE_SLOT( d_func(), void onTitleChanged( const QString& newTitle ) )
    Q_PRIVATE_SLOT( d_func(), void onLocalSyncStateChanged( Kasten::LocalSyncState newState ) )
//     void onViewFocusChanged( Kasten::AbstractView* view );
//     void onFocusRequested( Kasten::AbstractDocument* document );
    Q_PRIVATE_SLOT( d_func(), void onToolVisibilityChanged( bool isVisible ) )
//     Q_PRIVATE_SLOT( d_func(), void onCloseRequest( const QList<Kasten::AbstractView*>& views ) )
    Q_PRIVATE_SLOT( d_func(), void onDataOffered( const QMimeData* mimeData, bool& accept ) )
    Q_PRIVATE_SLOT( d_func(), void onDataDropped( const QMimeData* mimeData ) )

  protected:
    SingleViewWindowPrivate* const d_ptr;
    Q_DECLARE_PRIVATE( SingleViewWindow )
};

}

#endif
