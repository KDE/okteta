/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INSERTCONTROLLER_HPP
#define KASTEN_INSERTCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>

class KXMLGUIClient;
class KSelectAction;
class QAction;

namespace Kasten {

class ModelCodecViewManager;
class ModelCodecManager;
namespace If {
class SelectedDataWriteable;
}
class AbstractModelDataGenerator;

class InsertController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    InsertController(ModelCodecViewManager* modelCodecViewManager,
                     ModelCodecManager* modelCodecManager,
                     KXMLGUIClient* guiClient);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private:
    void updateActions();

private Q_SLOTS:
    void onReadOnlyChanged(bool isReadOnly);
    void onActionTriggered(QAction* action);
    void triggerExecution(Kasten::AbstractModelDataGenerator* generator);

private:
    ModelCodecViewManager* mModelCodecViewManager;
    ModelCodecManager* mModelCodecManager;

    AbstractModel* mModel = nullptr;
    If::SelectedDataWriteable* mSelectedDataWriteableControl = nullptr;

    KSelectAction* mInsertSelectAction;
};

}

#endif
