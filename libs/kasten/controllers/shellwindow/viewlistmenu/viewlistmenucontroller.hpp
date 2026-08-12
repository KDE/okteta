/*
    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWLISTMENUCONTROLLER_HPP
#define KASTEN_VIEWLISTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/XmlGuiActionListsPlugRequiring>

class KXMLGUIClient;
class QAction;
class QActionGroup;

namespace Kasten {

class ViewManager;
class AbstractGroupedViews;

class ViewListMenuController : public AbstractXmlGuiController
                             , public If::XmlGuiActionListsPlugRequiring
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::XmlGuiActionListsPlugRequiring
    )

public:
    ViewListMenuController(ViewManager* viewManager, AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

public: // If::XmlGuiActionListsPlugRequiring API
    void plugActionLists() override;

private Q_SLOTS:
    void updateActions();
    void onActionTriggered(QAction* action);

private:
    void plugViewListActionList();

private:
    ViewManager* const mViewManager;
    AbstractGroupedViews* const mGroupedViews;
    KXMLGUIClient* const mGuiClient;

    QActionGroup* mWindowsActionGroup;
};

}

#endif
