/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CROPCONTROLLER_HPP
#define KASTEN_CROPCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

namespace Okteta {
class AbstractByteArrayModel;
}
class KXMLGUIClient;
class QAction;

namespace Kasten {
class ByteArrayView;

class CropController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit CropController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void crop();

    void updateActionEnabled();

private:
    ByteArrayView* m_byteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* m_byteArrayModel = nullptr;

    QAction* m_cropAction;
};

}

#endif
