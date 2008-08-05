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
#include <kabstractview.h>
#include <kidataselectable.h>
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
// Qt
#include <QtGui/QActionGroup>

static const char ExportActionListId[] = "export_list";

ExportController::ExportController( KDocumentManager *documentManager, KXmlGuiWindow *window )
 : mDocumentManager( documentManager ), mWindow( window ), mView( 0 )
{
    mExportActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
    connect( mExportActionGroup, SIGNAL(triggered( QAction* )), SLOT(onActionTriggered( QAction* )) );

    setView( 0 );
}

void ExportController::setView( KAbstractView *view )
{
    if( mView ) mView->disconnect( mExportActionGroup );

    mView = view;
    mSelectionControl = view ? qobject_cast<KDE::If::DataSelectable *>( view ) : 0;

    if( mSelectionControl )
    {
        connect( mView, SIGNAL(hasSelectedDataChanged( bool )), SLOT(updateActions()) );
    }

    updateActions();
}

Q_DECLARE_METATYPE( AbstractModelExporter* )

void ExportController::updateActions()
{
    mWindow->unplugActionList( ExportActionListId );

    qDeleteAll( mExportActionGroup->actions() );

    const AbstractModelSelection *selection = ( mSelectionControl != 0 ) ? mSelectionControl->selection() : 0;

    const QList<AbstractModelExporter*> exporterList =
        mDocumentManager->codecManager()->exporterList( mView, selection );
    const bool hasExporters = ( exporterList.size() > 0 );

    if( hasExporters )
    {
        for( int c = 0; c < exporterList.size(); ++c )
        {
            AbstractModelExporter *exporter = exporterList.at( c );
            const QString title = exporter->remoteTypeName();
            QAction *action = new QAction( title, mExportActionGroup );

            action->setData( QVariant::fromValue(exporter) );
            mExportActionGroup->addAction( action );
        }
    }
    else
    {
        QAction *noneAction = new QAction( i18nc("@item There are no exporters.","Not available."), mExportActionGroup );
        mExportActionGroup->addAction( noneAction );
    }
    mExportActionGroup->setEnabled( hasExporters );

    mWindow->plugActionList( ExportActionListId, mExportActionGroup->actions() );
}

void ExportController::onActionTriggered( QAction *action )
{
    AbstractModelExporter *exporter = action->data().value<AbstractModelExporter *>();

    const AbstractModelSelection *selection = ( mSelectionControl != 0 ) ? mSelectionControl->selection() : 0;

    mDocumentManager->codecManager()->exportDocument( exporter, mView, selection );
}
