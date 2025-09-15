/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printcontroller.hpp"

// controller
#include "printtool.hpp"
// KF
#include <KXMLGUIClient>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QAction>

namespace Kasten {

PrintController::PrintController(KXMLGUIClient* guiClient, AbstractUserMessagesHandler* userMessagesHandler)
    : mPrintTool(std::make_unique<PrintTool>(userMessagesHandler))
{
    mPrintAction = KStandardAction::print(mPrintTool.get(), &PrintTool::print, this);
    mPrintAction->setEnabled(false);
    connect(mPrintTool.get(), &PrintTool::viewChanged,
            mPrintAction, &QAction::setEnabled);
    mPrintPreviewAction = KStandardAction::printPreview(mPrintTool.get(), &PrintTool::printPreview, this);
    mPrintPreviewAction->setEnabled(false);
    connect(mPrintTool.get(), &PrintTool::viewChanged,
            mPrintPreviewAction, &QAction::setEnabled);

    guiClient->actionCollection()->addActions({
        mPrintAction,
        mPrintPreviewAction,
    });
}

PrintController::~PrintController() = default;

void PrintController::setTargetModel(AbstractModel* model)
{
    mPrintTool->setTargetModel(model);
}

void PrintController::print()
{
    mPrintTool->print();
}

}

#include "moc_printcontroller.cpp"
