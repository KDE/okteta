/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OVERWRITEONLYCONTROLLER_HPP
#define KASTEN_OVERWRITEONLYCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

namespace Okteta {
class AbstractByteArrayModel;
}
class KXMLGUIClient;
class KToggleAction;

namespace Kasten {

class OverwriteOnlyController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit OverwriteOnlyController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void setOverwriteOnly(bool isOverwriteOnly);

private:
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    KToggleAction* mSetOverwriteOnlyAction;
};

}

#endif
