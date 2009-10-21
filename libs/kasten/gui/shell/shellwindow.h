/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SHELLWINDOW_H
#define SHELLWINDOW_H

// Kasten gui
#include "kastengui_export.h"
#include "widgetsdockable.h"
// Kasten core
#include <abstractdocument.h>
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QList>

class QMimeData;


namespace Kasten
{

class DocumentManager;
class AbstractView;
class ViewManager;
class MultiViewAreas;
class AbstractXmlGuiController;
class AbstractToolView;
class AbstractTool;


class KASTENGUI_EXPORT ShellWindow : public KXmlGuiWindow,
                                     public If::WidgetsDockable
{
   Q_OBJECT
   Q_INTERFACES( Kasten::If::WidgetsDockable )

  public:
    ShellWindow( DocumentManager* documentManager, ViewManager *viewManager );
    virtual ~ShellWindow();

  public:
    void updateControllers( AbstractView *view );
    void addXmlGuiController( AbstractXmlGuiController* controller );
    void addTool( AbstractToolView* toolView );

  public: // If::WidgetsDockable API
    virtual QList<ToolViewDockWidget*> dockWidgets() const;

  protected: // KMainWindow API
    virtual bool queryClose();

  private Q_SLOTS:
    void onTitleChanged( const QString &newTitle );
    void onSyncStatesChanged( Kasten::AbstractDocument::SyncStates newStates );
    void onViewFocusChanged( Kasten::AbstractView* view );
    void onFocusRequested( Kasten::AbstractDocument* document );
    void onToolVisibilityChanged( bool isVisible );
    void onCloseRequest( const QList<Kasten::AbstractView*>& views );
    void onDataOffered( const QMimeData* mimeData, bool& accept );
    void onDataDropped( const QMimeData* mimeData );

  protected:
    MultiViewAreas* mGroupedViews;
    // hack:
    // used to store a pointer to the current, so we can disconnect to its signals... well, not perfect
    AbstractView *mCurrentView;

    DocumentManager* mDocumentManager;
    ViewManager *mViewManager;
    QList<AbstractXmlGuiController*> mControllers;

    QList<ToolViewDockWidget*> mDockWidgets;
    QList<AbstractTool*> mTools;
};

}

#endif
