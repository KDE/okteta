/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

private Q_SLOTS: // execute slots
    void addNewViewProfile(const Kasten::ByteArrayViewProfile& viewProfile);

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
