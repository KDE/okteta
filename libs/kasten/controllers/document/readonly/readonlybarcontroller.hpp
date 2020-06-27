/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_READONLYBARCONTROLLER_HPP
#define KASTEN_READONLYBARCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

namespace Kasten {

class ToggleButton;
class AbstractDocument;
class StatusBar;

class ReadOnlyBarController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ReadOnlyBarController(StatusBar* statusBar);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void setReadOnly(bool isReadOnly);

private:
    AbstractDocument* mDocument = nullptr;

    ToggleButton* mReadOnlyButton;
};

}

#endif
