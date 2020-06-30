/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_READONLYCONTROLLER_HPP
#define KASTEN_READONLYCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KToggleAction;

namespace Kasten {

class AbstractDocument;

class ReadOnlyController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ReadOnlyController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void setReadOnly(bool isReadOnly);

private:
    AbstractDocument* mDocument = nullptr;

    KToggleAction* mSetReadOnlyAction;
};

}

#endif
