/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectrangecontroller.hpp"

// controller
#include "selectrangetoolview.hpp"
#include "selectrangetool.hpp"
// Kasten gui
#include <Kasten/ToolInlineViewable>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
// Qt
#include <QAction>

namespace Kasten {

// TODO: a tool(view) might perhaps be invoked indirectly by asking for a tool for the object, here select
// so the status bar selection indicator can invoke the same tool, without knowing about it
SelectRangeController::SelectRangeController(If::ToolInlineViewable* toolInlineViewable, KXMLGUIClient* guiClient)
    : mToolInlineViewable(toolInlineViewable)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSelectAction = new QAction(QIcon::fromTheme(QStringLiteral("select-rectangular")),
                                i18nc("@action:inmenu", "&Select Range…"), this);
    connect(mSelectAction, &QAction::triggered, this, &SelectRangeController::select);
    actionCollection->setDefaultShortcut(mSelectAction, QKeySequence(Qt::CTRL | Qt::Key_E));

    actionCollection->addAction(QStringLiteral("edit_select"), mSelectAction);

    mTool = std::make_unique<SelectRangeTool>();
    connect(mTool.get(), &SelectRangeTool::isUsableChanged,
            mSelectAction, &QAction::setEnabled);
    mSelectAction->setEnabled(mTool->isUsable());

    mView = std::make_unique<SelectRangeToolView>(mTool.get());
}

SelectRangeController::~SelectRangeController()
{
    if (mToolInlineViewable->currentToolInlineView() == mView.get()) {
        mToolInlineViewable->setCurrentToolInlineView(nullptr);
    }
}

void SelectRangeController::setTargetModel(AbstractModel* model)
{
    mTool->setTargetModel(model);
}

void SelectRangeController::select()
{
//     mView->activate(); // TODO: show would be better here, or should instead toolInlineViewable be asked?
    mToolInlineViewable->setCurrentToolInlineView(mView.get());
}

}

#include "moc_selectrangecontroller.cpp"
