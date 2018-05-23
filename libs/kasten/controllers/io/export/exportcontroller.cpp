/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "exportcontroller.h"

// lib
#include "exportdialog.h"
// Kasten gui
#include <kasten/modelcodecviewmanager.h>
#include <kasten/dataselectable.h>
#include <kasten/abstractmodelexporterconfigeditor.h>
// Kasten core
#include <kasten/modelcodecmanager.h>
#include <kasten/abstractdocument.h>
#include <kasten/abstractmodelexporter.h>
// KF5
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>

Q_DECLARE_METATYPE(Kasten::AbstractModelExporter*)

namespace Kasten {

ExportController::ExportController(ModelCodecViewManager* modelCodecViewManager,
                                   ModelCodecManager* modelCodecManager,
                                   KXMLGUIClient* guiClient)
    : AbstractXmlGuiController()
    , mModelCodecViewManager(modelCodecViewManager)
    , mModelCodecManager(modelCodecManager)
{
    mExportSelectAction = new KSelectAction(QIcon::fromTheme(QStringLiteral("document-export")),
                                            i18nc("@title:menu", "Export"),
                                            this);
    mExportSelectAction->setToolBarMode(KSelectAction::MenuMode);
    connect(mExportSelectAction, QOverload<QAction*>::of(&KSelectAction::triggered),
            this, &ExportController::onActionTriggered);

    guiClient->actionCollection()->addAction(QStringLiteral("export"), mExportSelectAction);

    setTargetModel(nullptr);
}

void ExportController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::DataSelectable*>() : nullptr;
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

    const QList<AbstractModelExporter*> exporterList =
        mModelCodecManager->exporterList(mModel, selection);
    const bool hasExporters = (exporterList.size() > 0);

    if (hasExporters) {
        for (int c = 0; c < exporterList.size(); ++c) {
            AbstractModelExporter* exporter = exporterList.at(c);
            const QString title = exporter->remoteTypeName();
            QAction* action = new QAction(title, mExportSelectAction);

            action->setData(QVariant::fromValue(exporter));
            mExportSelectAction->addAction(action);
        }
    } else {
        QAction* noneAction = new QAction(i18nc("@item There are no exporters.", "Not available."), mExportSelectAction);
        noneAction->setEnabled(false);
        mExportSelectAction->addAction(noneAction);
    }

    mExportSelectAction->setEnabled(mModel != nullptr);
}

void ExportController::onActionTriggered(QAction* action)
{
    AbstractModelExporter* exporter = action->data().value<AbstractModelExporter*>();

    const AbstractModelSelection* selection = mSelectionControl ? mSelectionControl->modelSelection() : nullptr;

    AbstractModelExporterConfigEditor* configEditor =
        mModelCodecViewManager->createConfigEditor(exporter);

    if (configEditor) {
        ExportDialog* dialog = new ExportDialog(exporter->remoteTypeName(), configEditor);
        dialog->setData(mModel, selection);
        if (!dialog->exec()) {
            return;
        }
    }

    mModelCodecManager->exportDocument(exporter, mModel, selection);
}

}
