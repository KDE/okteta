/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "creatorcontroller.hpp"

// Kasten gui
#include <Kasten/AbstractDocumentStrategy>
// Kasten core
#include <Kasten/ModelCodecManager>
#include <Kasten/AbstractModelDataGenerator>
// KF
#include <KLocalizedString>
#include <KActionCollection>
#include <KActionMenu>
#include <KXMLGUIClient>

namespace Kasten {

CreatorController::CreatorController(ModelCodecManager* modelCodecManager,
                                     AbstractDocumentStrategy* documentStrategy,
                                     KXMLGUIClient* guiClient)
    : mModelCodecManager(modelCodecManager)
    , mDocumentStrategy(documentStrategy)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    auto* newMenuAction =
        new KActionMenu(QIcon::fromTheme(QStringLiteral("document-new")),
                        i18nc("@title:menu create new byte arrays from different sources",
                              "New"),
                        this);
    newMenuAction->setPopupMode(QToolButton::MenuButtonPopup);
    actionCollection->setDefaultShortcuts(newMenuAction, KStandardShortcut::openNew());
    connect(newMenuAction, &QAction::triggered,
            this, &CreatorController::onNewActionTriggered);

    auto* newEmptyDocumentAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-new")), i18nc("@item:inmenu create a new empty document", "Empty"), this);
//     newEmptyDocumentAction->setToolTip( factory-toolTip() );
//         i18nc( "@info:tooltip", "Create an empty document" ) );
//     newEmptyDocumentAction->setHelpText( factory->helpText() );
//         i18nc( "@info:status", "Create an new, empty document" ) );
//     newEmptyDocumentAction->setWhatsThis( factory->whatsThis() );
//         i18nc( "@info:whatsthis", "." ) );
    connect(newEmptyDocumentAction, &QAction::triggered,
            this, &CreatorController::onNewActionTriggered);

    auto* newFromClipboardDocumentAction =
        new QAction(QIcon::fromTheme(QStringLiteral("edit-paste")), i18nc("@item:inmenu create a new document from data in the clipboard", "From Clipboard"), this);
    connect(newFromClipboardDocumentAction, &QAction::triggered,
            this, &CreatorController::onNewFromClipboardActionTriggered);

    newMenuAction->addAction(newEmptyDocumentAction);
    newMenuAction->addSeparator();
    newMenuAction->addAction(newFromClipboardDocumentAction);

    // generators
    const auto generatorList = mModelCodecManager->dataGenerators();

    const bool hasGenerators = (!generatorList.empty());

    if (hasGenerators) {
        newMenuAction->addSeparator();

        // TODO: ask factory which mimetypes it can read
        // AbstractDocumentFactory->canCreateFromData( QMimeData() ) needs already data
        for (AbstractModelDataGenerator* generator : generatorList) {
            const QString title = generator->typeName();
            const QString iconName = QStringLiteral("document-new");  // generator->iconName();

            auto* action = new QAction(QIcon::fromTheme(iconName), title, this);
            action->setData(QVariant::fromValue(generator));
            connect(action, &QAction::triggered,
                    this, &CreatorController::onNewFromGeneratorActionTriggered);

            newMenuAction->addAction(action);
        }
    }

    actionCollection->addAction(QStringLiteral("file_new"), newMenuAction);
}

CreatorController::~CreatorController() = default;

void CreatorController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void CreatorController::onNewActionTriggered()
{
    mDocumentStrategy->createNew();
}

void CreatorController::onNewFromClipboardActionTriggered()
{
    mDocumentStrategy->createNewFromClipboard();
}

void CreatorController::onNewFromGeneratorActionTriggered()
{
    auto* action = static_cast<QAction*>(sender());

    auto* generator = action->data().value<AbstractModelDataGenerator*>();

    mDocumentStrategy->createNewWithGenerator(generator);
}

}

#include "moc_creatorcontroller.cpp"
