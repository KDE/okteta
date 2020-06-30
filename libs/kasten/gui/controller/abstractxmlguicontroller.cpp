/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractxmlguicontroller.hpp"

namespace Kasten {

class AbstractXmlGuiControllerPrivate
{
public:
};


AbstractXmlGuiController::AbstractXmlGuiController() = default;

AbstractXmlGuiController::~AbstractXmlGuiController() = default;

void AbstractXmlGuiController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

}
