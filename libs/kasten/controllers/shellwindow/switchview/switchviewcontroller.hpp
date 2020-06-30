/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SWITCHVIEWCONTROLLER_HPP
#define KASTEN_SWITCHVIEWCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QAction;

namespace Kasten {

class AbstractGroupedViews;

class SwitchViewController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    SwitchViewController(AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void forward();
    void backward();

    void updateActions();

private:
    AbstractGroupedViews* mGroupedViews;

    QAction* mForwardAction;
    QAction* mBackwardAction;
};

}

#endif
