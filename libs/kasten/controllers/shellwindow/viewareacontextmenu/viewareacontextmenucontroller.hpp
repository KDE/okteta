/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWAREACONTEXTMENUCONTROLLER_HPP
#define KASTEN_VIEWAREACONTEXTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QPoint;
class QAction;

namespace Kasten {

class MultiViewAreas;
class AbstractGroupedViews;
class AbstractView;
class DocumentSyncManager;
class AbstractDocument;

class ViewAreaContextMenuController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    ViewAreaContextMenuController(MultiViewAreas* multiViewAreas,
                                  DocumentSyncManager* syncManager,
                                  KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // slots
    void showContextMenu(AbstractGroupedViews* viewArea, AbstractView* view, QPoint pos);

    void reload();
    void close();
    void closeAll();
    void closeAllOther();

private:
    AbstractGroupedViews* mViewArea = nullptr;
    AbstractView* mView = nullptr;
    AbstractDocument* mDocument = nullptr;

    KXMLGUIClient* const mGuiClient;
    MultiViewAreas* const mMultiViewAreas;
    DocumentSyncManager* const mSyncManager;

    QAction* mReloadAction;
    QAction* mCloseAction;
    QAction* mCloseAllAction;
    QAction* mCloseAllOtherAction;
};

}

#endif
