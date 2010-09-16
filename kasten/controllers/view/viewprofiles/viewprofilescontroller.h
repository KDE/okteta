/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWPROFILESCONTROLLER_H
#define VIEWPROFILESCONTROLLER_H

// lib
#include "oktetakastencontrollers_export.h"
// Kasten gui
#include <abstractxmlguicontroller.h>

class KXMLGUIClient;
class KActionMenu;
class QAction;
class QActionGroup;


namespace Kasten
{
class ByteArrayViewProfileManager;
class ByteArrayView;
class ViewProfile;


class OKTETAKASTENCONTROLLERS_EXPORT ViewProfilesController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    explicit ViewProfilesController( KXMLGUIClient* guiClient,
                                     ByteArrayViewProfileManager* viewProfileManager,
                                     QWidget* parentWidget );

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  protected:
    void updateViewProfileActions();

  protected Q_SLOTS: // update slots
    void onViewSettingsChanged();
    void onViewProfilesAdded( const QList<ViewProfile>& viewProfiles );
    void onViewProfilesChanged( const QList<ViewProfile>& viewProfiles );
    void onViewProfilesRemoved( const QList<ViewProfile>& viewProfiles );

  protected Q_SLOTS: // action slots
    void onCreateNewActionTriggered();
    void onResetChangesActionTriggered();
    void onSaveChangesActionTriggered();
    void onSetViewProfileActionTriggered();
    void onViewProfileTriggered( QAction* action );

  protected:
    KXMLGUIClient* mGuiClient;
    ByteArrayViewProfileManager* mViewProfileManager;
    QWidget* mParentWidget;

    ByteArrayView* mByteArrayView;

    // view menu
    KActionMenu* mViewProfileMenuAction;
    QAction* mCreateNewAction;
    QAction* mResetChangesAction;
    QAction* mSaveChangesAction;

    QActionGroup* mViewProfilesActionGroup;
};

}

#endif
