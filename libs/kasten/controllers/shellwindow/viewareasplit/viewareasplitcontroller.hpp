/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWAREASPLITCONTROLLER_HPP
#define KASTEN_VIEWAREASPLITCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QAction;

namespace Kasten {

namespace If {
class ViewAreaSplitable;
}
class AbstractGroupedViews;
class AbstractViewArea;
class ViewManager;

class ViewAreaSplitController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    ViewAreaSplitController(ViewManager* viewManager, AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private:
    void splitViewArea(Qt::Orientation orientation);

private Q_SLOTS:
    void splitVertically();
    void splitHorizontally();
    void close();

    void onViewAreaFocusChanged(Kasten::AbstractViewArea* viewArea);
    void onViewAreasChanged();
    void onViewsChanged();

private:
    ViewManager* mViewManager;
    AbstractGroupedViews* mGroupedViews;
    If::ViewAreaSplitable* mViewAreaSplitable = nullptr;
    AbstractGroupedViews* mCurrentViewArea = nullptr;

    QAction* mSplitVerticallyAction;
    QAction* mSplitHorizontallyAction;
    QAction* mCloseAction;
};

}

#endif
