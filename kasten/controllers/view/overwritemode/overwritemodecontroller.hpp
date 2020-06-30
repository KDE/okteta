/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OVERWRITEMODECONTROLLER_HPP
#define KASTEN_OVERWRITEMODECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KToggleAction;

namespace Kasten {

class ByteArrayView;

class OverwriteModeController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit OverwriteModeController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void setOverWrite(bool isOverWrite);

private:
    ByteArrayView* mByteArrayView = nullptr;

    KToggleAction* mSetOverWriteAction;
};

}

#endif
