/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWMODECONTROLLER_HPP
#define KASTEN_VIEWMODECONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class KSelectAction;

namespace Kasten {

class ByteArrayView;

class ViewModeController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ViewModeController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void setViewMode(int viewMode);

    void onViewModusChanged(int viewModus);

private:
    ByteArrayView* mByteArrayView = nullptr;

    KSelectAction* mViewModeAction;
};

}

#endif
