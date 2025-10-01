/*
    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "gotooffsetcontroller.hpp"

// controller
#include "gotooffsettoolview.hpp"
#include "gotooffsettool.hpp"
// Kasten gui
#include <Kasten/ToolInlineViewable>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
// Qt
#include <QAction>

namespace Kasten {

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
GotoOffsetController::GotoOffsetController(If::ToolInlineViewable* toolInlineViewable, KXMLGUIClient* guiClient)
    : mToolInlineViewable(toolInlineViewable)
{
    KActionCollection* const actionCollection = guiClient->actionCollection();

    mGotoOffsetAction = new QAction(QIcon::fromTheme(QStringLiteral("go-jump")),
                                    i18nc("@action:inmenu", "&Go to Offset…"), this);
    connect(mGotoOffsetAction, &QAction::triggered,this, &GotoOffsetController::gotoOffset);
    actionCollection->setDefaultShortcut(mGotoOffsetAction, QKeySequence(Qt::CTRL | Qt::Key_G));

    actionCollection->addAction(QStringLiteral("goto_offset"), mGotoOffsetAction);

    mTool = std::make_unique<GotoOffsetTool>();
    connect(mTool.get(), &GotoOffsetTool::isUsableChanged,
            mGotoOffsetAction, &QAction::setEnabled);
    mGotoOffsetAction->setEnabled(mTool->isUsable());

    mView = std::make_unique<GotoOffsetToolView>(mTool.get());
}

GotoOffsetController::~GotoOffsetController()
{
    if (mToolInlineViewable->currentToolInlineView() == mView.get()) {
        mToolInlineViewable->setCurrentToolInlineView(nullptr);
    }
}

void GotoOffsetController::setTargetModel(AbstractModel* model)
{
    mTool->setTargetModel(model);
}

void GotoOffsetController::gotoOffset()
{
    mToolInlineViewable->setCurrentToolInlineView(mView.get());
}

}

#include "moc_gotooffsetcontroller.cpp"
