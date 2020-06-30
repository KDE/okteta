/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_QUITCONTROLLER_HPP
#define KASTEN_QUITCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXmlGuiWindow;

namespace Kasten {

class QuitController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit QuitController(KXmlGuiWindow* window);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void quit();

private:
    KXmlGuiWindow* mMainWindow;
};

}

#endif
