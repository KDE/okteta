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

#ifndef KASTEN_VIEWPROFILECONTROLLER_HPP
#define KASTEN_VIEWPROFILECONTROLLER_HPP

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayViewProfile>
// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Kasten core
#include <Kasten/KastenCore>

class KXMLGUIClient;
class KActionMenu;
class QActionGroup;
class QAction;

namespace Kasten {
class ByteArrayViewProfileManager;
class ByteArrayViewProfileSynchronizer;
class ByteArrayView;

class ViewProfileController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    ViewProfileController(ByteArrayViewProfileManager* viewProfileManager,
                          QWidget* parentWidget,
                          KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // update slots
    void onViewProfileChanged(const Kasten::ByteArrayViewProfile::Id& viewProfileId);
    void onLocalSyncStateChanged(Kasten::LocalSyncState localSyncState);

    void onViewProfilesChanged();

private Q_SLOTS: // action slots
    void onCreateNewActionTriggered();
    void onResetChangesActionTriggered();
    void onSaveChangesActionTriggered();
    void onViewProfileTriggered(QAction* action);

private:
    KXMLGUIClient* mGuiClient;
    ByteArrayViewProfileManager* mViewProfileManager;
    QWidget* mParentWidget;

    ByteArrayView* mByteArrayView = nullptr;
    ByteArrayViewProfileSynchronizer* mByteArrayViewProfileSynchronizer = nullptr;

    // menu
    KActionMenu* mViewProfileActionMenu;

    QAction* mCreateNewAction;
    QAction* mResetChangesAction;
    QAction* mSaveChangesAction;
    QActionGroup* mViewProfilesActionGroup;
};

}

#endif
