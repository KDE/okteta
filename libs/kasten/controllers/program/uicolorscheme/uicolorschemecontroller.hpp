/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UICOLORSCHEMECONTROLLER_HPP
#define KASTEN_UICOLORSCHEMECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXmlGuiWindow;
class QAction;

namespace Kasten {

class UiColorSchemeController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit UiColorSchemeController(KXmlGuiWindow* window);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private:
    void handleSchemeChanged(QAction* action);
};

}

#endif
