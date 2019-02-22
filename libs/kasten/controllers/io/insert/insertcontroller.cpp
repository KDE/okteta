/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "insertcontroller.hpp"

// lib
#include "insertdialog.hpp"
// Kasten gui
#include <Kasten/ModelCodecViewManager>
#include <Kasten/SelectedDataWriteable>
#include <Kasten/AbstractModelDataGeneratorConfigEditor>
// Kasten core
#include <Kasten/ModelDataGenerateThread>
#include <Kasten/ModelCodecManager>
#include <Kasten/AbstractModelDataGenerator>
#include <Kasten/AbstractModel>
// KF5
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>
// Qt
#include <QMimeData>
#include <QApplication>

#ifndef KASTEN_ABSTRACTMODELDATAGENERATOR_METATYPE
#define KASTEN_ABSTRACTMODELDATAGENERATOR_METATYPE
Q_DECLARE_METATYPE(Kasten::AbstractModelDataGenerator*)
#endif

namespace Kasten {

InsertController::InsertController(ModelCodecViewManager* modelCodecViewManager,
                                   ModelCodecManager* modelCodecManager,
                                   KXMLGUIClient* guiClient)
    : AbstractXmlGuiController()
    , mModelCodecViewManager(modelCodecViewManager)
    , mModelCodecManager(modelCodecManager)
{
    mInsertSelectAction = new KSelectAction(i18nc("@title:menu", "Insert"), this);
//     mInsertSelectAction->setIcon( QIcon::fromTheme( QStringLiteral("insert-text") ) );
    mInsertSelectAction->setToolBarMode(KSelectAction::MenuMode);
    connect(mInsertSelectAction, QOverload<QAction*>::of(&KSelectAction::triggered),
            this, &InsertController::onActionTriggered);

    // TODO: find better id
    guiClient->actionCollection()->addAction(QStringLiteral("insert"), mInsertSelectAction);

    setTargetModel(nullptr);
}

void InsertController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::SelectedDataWriteable*>() : nullptr;
    mSelectedDataWriteableControl = mModel ? qobject_cast<If::SelectedDataWriteable*>(mModel) : nullptr;

    if (mSelectedDataWriteableControl) {
        // TODO: only fill the list on menu call...
        connect(mModel, &AbstractModel::readOnlyChanged,
                this, &InsertController::onReadOnlyChanged);
    }

    updateActions();
}

void InsertController::updateActions()
{
    mInsertSelectAction->removeAllActions();

    // TODO: pass model to find which mimetypes it can read
    // mSelectedDataWriteableControl->canReadData( QMimeData() ) needs already data
    // TODO: it this depend on the current selection/focus? So it needs to be updated on every change?
    const QVector<AbstractModelDataGenerator*> generatorList =
        mModelCodecManager->generatorList();
    const bool hasGenerators = (generatorList.size() > 0);

    if (hasGenerators) {
        for (AbstractModelDataGenerator* generator : generatorList) {
            const QString title = generator->typeName();
            auto* action = new QAction(title, mInsertSelectAction);

            action->setData(QVariant::fromValue(generator));
            mInsertSelectAction->addAction(action);
        }
    } else {
        QAction* noneAction = new QAction(i18nc("@item There are no generators.", "Not available."), mInsertSelectAction);
        noneAction->setEnabled(false);
        mInsertSelectAction->addAction(noneAction);
    }

    // TODO: need a call AbstractModelSelection::isEmpty
    const bool isWriteable = (mSelectedDataWriteableControl && !mModel->isReadOnly());
    mInsertSelectAction->setEnabled(isWriteable);
}

void InsertController::onActionTriggered(QAction* action)
{
    auto* generator = action->data().value<AbstractModelDataGenerator*>();

    AbstractModelDataGeneratorConfigEditor* configEditor =
        mModelCodecViewManager->createConfigEditor(generator);

    if (configEditor) {
        auto* dialog = new InsertDialog(configEditor);
//         dialog->setData( mModel, selection ); TODO
        if (!dialog->exec()) {
            return;
        }
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* generateThread = new ModelDataGenerateThread(this, generator);
    generateThread->start();
    while (!generateThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    QMimeData* mimeData = generateThread->data();

    delete generateThread;

    mSelectedDataWriteableControl->insertData(mimeData);

    QApplication::restoreOverrideCursor();
}

void InsertController::onReadOnlyChanged(bool isReadOnly)
{
    const bool isWriteable = (!isReadOnly);

    mInsertSelectAction->setEnabled(isWriteable);
}

}
