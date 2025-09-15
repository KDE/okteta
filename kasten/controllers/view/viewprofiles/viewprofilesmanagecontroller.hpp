/*
    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWPROFILESMANAGECONTROLLER_HPP
#define KASTEN_VIEWPROFILESMANAGECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QAction;

namespace Kasten {
class ByteArrayViewProfileManager;

class ViewProfilesManageController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ViewProfilesManageController(KXMLGUIClient* guiClient,
                                          ByteArrayViewProfileManager* viewProfileManager,
                                          QWidget* parentWidget);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void manageProfiles();

private:
    ByteArrayViewProfileManager* const mViewProfileManager;
    QWidget* const mParentWidget;

    QAction* mManageAction;
};

}

#endif
