/*
    This file is part of the Kakao Framework, part of the KDE project.

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


// Kakao gui
#include "iwidgetsdockable.h"
// Kakao core
#include <kabstractdocument.h>
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QList>

class KDocumentManager;
class KAbstractView;
class KViewManager;
class ParallelViews;
class AbstractXmlGuiController;
class AbstractToolView;
class AbstractTool;

class ShellWindow : public KXmlGuiWindow, public KDE::If::WidgetsDockable
{
   Q_OBJECT
   Q_INTERFACES( KDE::If::WidgetsDockable )

  public:
    ShellWindow( KDocumentManager *documentManager, KViewManager *viewManager );
    virtual ~ShellWindow();

  public:
    void updateControllers( KAbstractView *view );
    void addXmlGuiController( AbstractXmlGuiController* controller );
    void addTool( AbstractToolView* toolView );

  public: // KDE::If::WidgetsDockable API
    virtual QList<QDockWidget*> dockWidgets() const;

  protected: // KMainWindow API
    virtual bool queryClose();

  private Q_SLOTS:
    void onTitleChanged( const QString &newTitle );
    void onModifiedChanged( KAbstractDocument::SynchronizationStates newStates );
    void onViewFocusChanged( KAbstractView *view );

  protected:
    ParallelViews* mGroupedViews;
    // hack:
    // used to store a pointer to the current, so we can disconnect to its signals... well, not perfect
    KAbstractView *mCurrentView;

    KDocumentManager *mDocumentManager;
    KViewManager *mViewManager;
    QList<AbstractXmlGuiController*> mControllers;

    QList<QDockWidget*> mDockWidgets;
    QList<AbstractToolView*> mToolViews;
    QList<AbstractTool*> mTools;
};

#endif
