/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWCONTEXTMENUCONTROLLER_HPP
#define KASTEN_VIEWCONTEXTMENUCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QPoint;

namespace Kasten {

class ByteArrayView;

class ViewContextMenuController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ViewContextMenuController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // slots
    void showContextMenu(QPoint pos);

private:
    ByteArrayView* mByteArrayView = nullptr;

    KXMLGUIClient* const mGuiClient;
};

}

#endif
