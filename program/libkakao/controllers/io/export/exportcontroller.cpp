/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// Kakao gui
#include <modelcodecviewmanager.h>
#include <kidataselectable.h>
#include <kviewmanager.h>
#include <abstractmodelexporterconfigeditor.h>
// Kakao core
#include <kdocumentmanager.h>
#include <modelcodecmanager.h>
#include <kabstractdocument.h>
#include <abstractmodelexporter.h>
// KDE
#include <KXmlGuiWindow>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocale>
#include <KSelectAction>


ExportController::ExportController( KViewManager* viewManager, KDocumentManager* documentManager, KXMLGUIClient* guiClient )
 : mViewManager( viewManager ), mDocumentManager( documentManager ), mGuiClient( guiClient ), mModel( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mExportSelectAction = actionCollection->add<KSelectAction>( "export" );
    mExportSelectAction->setText( i18nc("@title:menu","Export") );
    mExportSelectAction->setIcon( KIcon("document-export") );
    mExportSelectAction->setToolBarMode( KSelectAction::MenuMode );
    connect( mExportSelectAction, SIGNAL(triggered( QAction* )), SLOT(onActionTriggered( QAction* )) );

    setTargetModel( 0 );
}

void ExportController::setTargetModel( AbstractModel* model )
{
    if( mModel ) mModel->disconnect( this );

    mModel = model ? model->findBaseModelWithInterface<KDE::If::DataSelectable*>() : 0;
    mSelectionControl = mModel ? qobject_cast<KDE::If::DataSelectable *>( mModel ) : 0;

    if( mSelectionControl )
    {
        // TODO: only fill the list on menu call...
        connect( mModel, SIGNAL(hasSelectedDataChanged( bool )), SLOT(updateActions()) );
    }

    updateActions();
}

Q_DECLARE_METATYPE( AbstractModelExporter* )

void ExportController::updateActions()
{
    mExportSelectAction->removeAllActions();

    const AbstractModelSelection* selection = ( mSelectionControl != 0 ) ? mSelectionControl->modelSelection() : 0;

    const QList<AbstractModelExporter*> exporterList =
        mDocumentManager->codecManager()->exporterList( mModel, selection );
    const bool hasExporters = ( exporterList.size() > 0 );

    if( hasExporters )
    {
        for( int c = 0; c < exporterList.size(); ++c )
        {
            AbstractModelExporter *exporter = exporterList.at( c );
            const QString title = exporter->remoteTypeName();
            QAction* action = new QAction( title, mExportSelectAction );

            action->setData( QVariant::fromValue(exporter) );
            mExportSelectAction->addAction( action );
        }
    }
    else
    {
        QAction* noneAction = new QAction( i18nc("@item There are no exporters.","Not available."), mExportSelectAction );
        noneAction->setEnabled( false );
        mExportSelectAction->addAction( noneAction );
    }

    mExportSelectAction->setEnabled( mModel != 0 );
}

void ExportController::onActionTriggered( QAction *action )
{
    AbstractModelExporter *exporter = action->data().value<AbstractModelExporter *>();

    const AbstractModelSelection* selection = ( mSelectionControl != 0 ) ? mSelectionControl->modelSelection() : 0;

    AbstractModelExporterConfigEditor* configEditor =
        mViewManager->codecViewManager()->createConfigEditor( exporter );

    if( configEditor )
    {
        ExportDialog* dialog = new ExportDialog( configEditor );
        dialog->setData( mModel, selection );
        if( !dialog->exec() )
            return;
    }

    mDocumentManager->codecManager()->exportDocument( exporter, mModel, selection );
}
