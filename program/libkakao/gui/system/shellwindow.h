/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <kviewcontroller.h>
// Kakao core
#include <kabstractdocument.h>
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QList>

class KDocumentManager;
class KAbstractView;
class KViewManager;
class TabbedViews;


class ShellWindow : public KXmlGuiWindow
{
   Q_OBJECT

  public:
    ShellWindow( KDocumentManager *documentManager, KViewManager *viewManager );
    virtual ~ShellWindow();

  public:
    void updateControllers( KAbstractView *view );

  protected: // KMainWindow API
    virtual bool queryClose();

  private Q_SLOTS:
    void onTitleChanged( const QString &newTitle );
    void onModifiedChanged( KAbstractDocument::SynchronizationStates newStates );
    void onViewFocusChanged( KAbstractView *view );

  protected:
    TabbedViews *mTabbedViews;
    // hack:
    // used to store a pointer to the current, so we can disconnect to its signals... well, not perfect
    KAbstractView *mCurrentView;

    KDocumentManager *mDocumentManager;
    KViewManager *mViewManager;
    QList<KViewController*> mControllers;
};

#endif
