/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONCONTROLLER_HPP
#define KASTEN_VERSIONCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class KToolBarPopupAction;
class QAction;

namespace Kasten {

namespace If {
class Versionable;
}

class VersionController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit VersionController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void onSetToOlderVersionTriggered();
    void onSetToNewerVersionTriggered();
    void onOlderVersionMenuAboutToShow();
    void onNewerVersionMenuAboutToShow();
    void onOlderVersionMenuTriggered(QAction* action);
    void onNewerVersionMenuTriggered(QAction* action);

    void onVersionIndexChanged(int versionIndex);
    void onReadOnlyChanged(bool isReadOnly);

private:
    AbstractModel* mModel = nullptr;
    If::Versionable* mVersionControl = nullptr;

    KToolBarPopupAction* mSetToOlderVersionAction;
    KToolBarPopupAction* mSetToNewerVersionAction;
};

}

#endif
