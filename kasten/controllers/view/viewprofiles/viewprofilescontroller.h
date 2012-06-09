/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <bytearrayviewprofile.h>
// Kasten gui
#include <abstractxmlguicontroller.h>
// Kasten core
#include <kastencore.h>

class KXMLGUIClient;
class QAction;
class QActionGroup;


namespace Kasten2
{
class ByteArrayViewProfileManager;
class ByteArrayViewProfileSynchronizer;
class ByteArrayView;


class OKTETAKASTENCONTROLLERS_EXPORT ViewProfilesController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    explicit ViewProfilesController( ByteArrayViewProfileManager* viewProfileManager,
                                     KXMLGUIClient* guiClient );

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  private Q_SLOTS: // update slots
    void onViewProfileChanged( const Kasten2::ByteArrayViewProfile::Id& viewProfileId );

    void onViewProfilesChanged();

  private Q_SLOTS: // action slots
    void onViewProfileTriggered( QAction* action );

  private:
    KXMLGUIClient* mGuiClient;
    ByteArrayViewProfileManager* mViewProfileManager;

    ByteArrayView* mByteArrayView;
    ByteArrayViewProfileSynchronizer* mByteArrayViewProfileSynchronizer;

    // view menu
    QAction* mCreateNewAction;
    QAction* mResetChangesAction;
    QAction* mSaveChangesAction;

    QActionGroup* mViewProfilesActionGroup;
};

}

#endif
