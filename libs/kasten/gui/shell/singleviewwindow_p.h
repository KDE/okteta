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

#ifndef SINGLEVIEWWINDOW_P_H
#define SINGLEVIEWWINDOW_P_H

// lib
#include "singleviewwindow.h"
// Kasten core
#include <abstractdocument.h>

class QDragMoveEvent;
class QDropEvent;


namespace Kasten
{

class SingleViewArea;
class AbstractTool;


class SingleViewWindowPrivate
{
  public:
    SingleViewWindowPrivate( SingleViewWindow* parent,
                             DocumentManager* documentManager, AbstractView* view );

    ~SingleViewWindowPrivate();

  public:
    void init();

  public:
    void setView( AbstractView* view );
    void addXmlGuiController( AbstractXmlGuiController* controller );
    void addTool( AbstractToolView* toolView );

  public: // If::WidgetsDockable API
    QList<ToolViewDockWidget*> dockWidgets() const;

  protected: // KMainWindow API
    bool queryClose();

  protected:
    AbstractView* view() const;
    SingleViewArea* viewArea() const;
    DocumentManager* documentManager() const;

  private: // Q_SLOTS
    void onTitleChanged( const QString& newTitle );
    void onLocalSyncStateChanged( Kasten::LocalSyncState newState );
    void onToolVisibilityChanged( bool isVisible );
    void onDataOffered( const QMimeData* mimeData, bool& accept );
    void onDataDropped( const QMimeData* mimeData );

  protected:
    Q_DECLARE_PUBLIC( SingleViewWindow )

  protected:
    SingleViewWindow* const q_ptr;

    DocumentManager* const mDocumentManager;

    AbstractView* mView;

    SingleViewArea* const mViewArea;
    QList<AbstractXmlGuiController*> mControllers;
    QList<ToolViewDockWidget*> mDockWidgets;
    QList<AbstractTool*> mTools;
};


inline QList<ToolViewDockWidget*> SingleViewWindowPrivate::dockWidgets() const { return mDockWidgets; }
inline AbstractView* SingleViewWindowPrivate::view() const { return mView; }
inline SingleViewArea* SingleViewWindowPrivate::viewArea() const { return mViewArea; }
inline DocumentManager* SingleViewWindowPrivate::documentManager() const { return mDocumentManager; }

}

#endif
