/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWLISTMENUCONTROLLER_HPP
#define KASTEN_VIEWLISTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class QAction;
class QActionGroup;
class KXMLGUIClient;

namespace Kasten {

class ViewManager;
class AbstractGroupedViews;

class ViewListMenuController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    ViewListMenuController(ViewManager* viewManager, AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void updateActions();
    void onActionTriggered(QAction* action);

private:
    ViewManager* mViewManager;
    AbstractGroupedViews* mGroupedViews;
    KXMLGUIClient* mGuiClient;

    QActionGroup* mWindowsActionGroup;
};

}

#endif
