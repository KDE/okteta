/*
    SPDX-FileCopyrightText: 2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "exportcontroller.hpp"

// lib
#include "exportdialog.hpp"
// Kasten gui
#include <Kasten/ModelCodecViewManager>
#include <Kasten/DataSelectable>
#include <Kasten/AbstractModelExporterConfigEditor>
// Kasten core
#include <Kasten/ModelCodecManager>
#include <Kasten/AbstractModel>
#include <Kasten/AbstractModelExporter>
#include <Kasten/AbstractModelSelection>
// KF
#include <KXMLGUIClient>
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>
// Qt
#include <QApplication>

namespace Kasten {

ExportController::ExportController(ModelCodecViewManager* modelCodecViewManager,
                                   ModelCodecManager* modelCodecManager,
                                   KXMLGUIClient* guiClient)
    : mModelCodecViewManager(modelCodecViewManager)
    , mModelCodecManager(modelCodecManager)
{
    mExportSelectAction = new KSelectAction(QIcon::fromTheme(QStringLiteral("document-export")),
                                            i18nc("@title:menu", "Export"),
                                            this);
    mExportSelectAction->setToolBarMode(KSelectAction::MenuMode);
    connect(mExportSelectAction, qOverload<QAction*>(&KSelectAction::triggered),
            this, &ExportController::onActionTriggered);

    guiClient->actionCollection()->addAction(QStringLiteral("export"), mExportSelectAction);

    updateActions();
}

void ExportController::setTargetModel(AbstractModel* model)
{
    AbstractModel* const dataSelectableModel = model ? model->findBaseModelWithInterface<If::DataSelectable*>() : nullptr;
    if (dataSelectableModel == mModel) {
        return;
    }

    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = dataSelectableModel;
    mSelectionControl = mModel ? qobject_cast<If::DataSelectable*>(mModel) : nullptr;

    if (mSelectionControl) {
        // TODO: only fill the list on menu call...
        connect(mModel, SIGNAL(hasSelectedDataChanged(bool)), SLOT(updateActions()));
    }

    updateActions();
}

void ExportController::updateActions()
{
    mExportSelectAction->removeAllActions();

    const AbstractModelSelection* selection = mSelectionControl ? mSelectionControl->modelSelection() : nullptr;

    auto exporterList = mModelCodecManager->exporters(mModel, selection);
    const bool hasExporters = (!exporterList.empty());

    if (hasExporters) {
        for (auto* exporter : exporterList) {
            const QString title = exporter->remoteTypeName();
            auto* action = new QAction(title, mExportSelectAction);

            action->setData(QVariant::fromValue(exporter));
            mExportSelectAction->addAction(action);
        }
    } else {
        auto* noneAction = new QAction(i18nc("@item There are no exporters.", "Not available."), mExportSelectAction);
        noneAction->setEnabled(false);
        mExportSelectAction->addAction(noneAction);
    }

    mExportSelectAction->setEnabled(mModel != nullptr);
}

void ExportController::onActionTriggered(QAction* action)
{
    auto* exporter = action->data().value<AbstractModelExporter*>();

    const AbstractModelSelection* selection = mSelectionControl ? mSelectionControl->modelSelection() : nullptr;

    auto configEditor = mModelCodecViewManager->createConfigEditor(exporter);

    if (configEditor) {
        auto* dialog = new ExportDialog(exporter->remoteTypeName(), std::move(configEditor), exporter,
                                        QApplication::activeWindow());
        dialog->setData(mModel, selection);
        connect(dialog, &ExportDialog::exportAccepted, this, &ExportController::triggerExecution);
        dialog->open();
        return;
    }

    triggerExecution(exporter, selection);
}

void ExportController::triggerExecution(AbstractModelExporter* exporter,
                                        const AbstractModelSelection* selection)
{
    mModelCodecManager->exportDocument(exporter, mModel, selection);
}

}

#include "moc_exportcontroller.cpp"
