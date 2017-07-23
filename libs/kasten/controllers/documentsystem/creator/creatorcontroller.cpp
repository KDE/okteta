/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "creatorcontroller.h"

// Kasten gui
#include <kasten/abstractdocumentstrategy.h>
#include <kasten/selecteddatawriteable.h>
// Kasten core
#include <kasten/modelcodecmanager.h>
#include <kasten/abstractmodeldatagenerator.h>
// KF5
#include <KLocalizedString>
#include <KActionCollection>
#include <KActionMenu>
#include <KStandardAction>
#include <KXMLGUIClient>
// Qt
#include <QApplication>
#include <QMimeData>


#ifndef ABSTRACTMODELDATAGENERATOR_METATYPE
#define ABSTRACTMODELDATAGENERATOR_METATYPE
Q_DECLARE_METATYPE(Kasten::AbstractModelDataGenerator*)
#endif

namespace Kasten
{

CreatorController::CreatorController( ModelCodecManager* modelCodecManager,
                                      AbstractDocumentStrategy* documentStrategy,
                                      KXMLGUIClient* guiClient )
  : AbstractXmlGuiController(),
    mModelCodecManager( modelCodecManager ),
    mDocumentStrategy( documentStrategy )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    KActionMenu* newMenuAction = actionCollection->add<KActionMenu>( QStringLiteral("file_new"), this, SLOT(onNewActionTriggered()) );
    newMenuAction->setText( i18nc("@title:menu create new byte arrays from different sources", "New" ) );
    newMenuAction->setIcon( QIcon::fromTheme( QStringLiteral("document-new") ) );
    newMenuAction->setStickyMenu( false );
    newMenuAction->setDelayed( false );
    actionCollection->setDefaultShortcuts( newMenuAction, KStandardShortcut::openNew() );

    QAction* newEmptyDocumentAction =
        new QAction( QIcon::fromTheme( QStringLiteral("document-new") ), i18nc("@title:menu create a new empty document", "Empty" ), this );
//     newEmptyDocumentAction->setToolTip( factory-toolTip() );
//         i18nc( "@info:tooltip", "Create an empty document" ) );
//     newEmptyDocumentAction->setHelpText( factory->helpText() );
//         i18nc( "@info:status", "Create an new, empty document" ) );
//     newEmptyDocumentAction->setWhatsThis( factory->whatsThis() );
//         i18nc( "@info:whatsthis", "." ) );
    connect( newEmptyDocumentAction, &QAction::triggered,
             this, &CreatorController::onNewActionTriggered );

    QAction* newFromClipboardDocumentAction =
        new QAction( QIcon::fromTheme( QStringLiteral("edit-paste") ), i18nc("@title:menu create a new document from data in the clipboard", "From Clipboard" ), this );
    connect( newFromClipboardDocumentAction, &QAction::triggered,
             this, &CreatorController::onNewFromClipboardActionTriggered );

    newMenuAction->addAction( newEmptyDocumentAction );
    newMenuAction->addSeparator();
    newMenuAction->addAction( newFromClipboardDocumentAction );

    // generators
    const QList<AbstractModelDataGenerator*> generatorList =
        mModelCodecManager->generatorList();

    const bool hasGenerators = ( generatorList.size() > 0 );

    if( hasGenerators )
    {
        newMenuAction->addSeparator();

        // TODO: ask factory which mimetypes it can read
        // AbstractDocumentFactory->canCreateFromData( QMimeData() ) needs already data
        foreach( AbstractModelDataGenerator* generator, generatorList )
        {
            const QString title = generator->typeName();
            const QString iconName = QString::fromLatin1( "document-new" );//generator->iconName();

            QAction* action = new QAction( QIcon::fromTheme(iconName), title, this );
            action->setData( QVariant::fromValue(generator) );
            connect( action, &QAction::triggered,
                     this, &CreatorController::onNewFromGeneratorActionTriggered );

            newMenuAction->addAction( action );
        }
    }
}

void CreatorController::setTargetModel( AbstractModel* model )
{
Q_UNUSED( model )
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
    QAction* action = static_cast<QAction*>( sender() );

    AbstractModelDataGenerator* generator = action->data().value<AbstractModelDataGenerator* >();

    mDocumentStrategy->createNewWithGenerator( generator );
}

CreatorController::~CreatorController() {}

}
