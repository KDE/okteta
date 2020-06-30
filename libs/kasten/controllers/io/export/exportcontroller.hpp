/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_EXPORTCONTROLLER_HPP
#define KASTEN_EXPORTCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class KSelectAction;
class QAction;

namespace Kasten {

class ModelCodecViewManager;
class ModelCodecManager;
namespace If {
class DataSelectable;
}
class AbstractModelExporter;
class AbstractModelSelection;

class ExportController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    ExportController(ModelCodecViewManager* modelCodecViewManager,
                     ModelCodecManager* modelCodecManager,
                     KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void updateActions();
    void onActionTriggered(QAction* action);
    void triggerExecution(Kasten::AbstractModelExporter* exporter,
                          const Kasten::AbstractModelSelection* selection);

private:
    ModelCodecViewManager* mModelCodecViewManager;
    ModelCodecManager* mModelCodecManager;

    AbstractModel* mModel = nullptr;
    If::DataSelectable* mSelectionControl = nullptr;

    KSelectAction* mExportSelectAction;
};

}

#endif
