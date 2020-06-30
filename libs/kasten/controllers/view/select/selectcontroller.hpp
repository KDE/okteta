/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SELECTCONTROLLER_HPP
#define KASTEN_SELECTCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class QAction;

namespace Kasten {

namespace If {
class DataSelectable;
}

class SelectController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit SelectController(KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void selectAll();
    void unselect();

private Q_SLOTS:
    void onHasSelectedDataChanged(bool hasSelectedData);

private:
    AbstractModel* mModel = nullptr;
    If::DataSelectable* mSelectControl = nullptr;

    QAction* mSelectAllAction;
    QAction* mDeselectAction;
};

}

#endif
