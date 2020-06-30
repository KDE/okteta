/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FULLSCREENCONTROLLER_HPP
#define KASTEN_FULLSCREENCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXmlGuiWindow;

namespace Kasten {

class FullScreenController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit FullScreenController(KXmlGuiWindow* window);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void switchFullScreen(bool toFullScreen);

private:
//     KProgram *Program;
    KXmlGuiWindow* mMainWindow;
};

}

#endif
