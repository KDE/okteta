/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

PrintController::PrintController(KXMLGUIClient* guiClient)
    : mPrintTool(new PrintTool())
{
    mPrintAction = KStandardAction::print(mPrintTool, &PrintTool::print, this);
    connect(mPrintTool, &PrintTool::viewChanged,
            mPrintAction, &QAction::setEnabled);

    guiClient->actionCollection()->addAction(mPrintAction->objectName(), mPrintAction);


    setTargetModel(nullptr);
}

PrintController::~PrintController()
{
    delete mPrintTool;
}

void PrintController::setTargetModel(AbstractModel* model)
{
    mPrintTool->setTargetModel(model);
}

void PrintController::print()
{
    mPrintTool->print();
}

}
