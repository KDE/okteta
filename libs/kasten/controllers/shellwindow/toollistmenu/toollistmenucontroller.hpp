/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLLISTMENUCONTROLLER_HPP
#define KASTEN_TOOLLISTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Qt
#include <QList>

class QAction;
class KXMLGUIClient;

namespace Kasten {

class ViewManager;

namespace If {
class WidgetsDockable;
}

class ToolListMenuController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    ToolListMenuController(If::WidgetsDockable* widgetsDockable, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private:
    void updateActions();

private:
    If::WidgetsDockable* mWidgetsDockable;
    KXMLGUIClient* mGuiClient;

    QList<QAction*> mToolActionList;
};

}

#endif
