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

#include "processdocumentconnectdialog.h"

// tool
#include "processdocumenttablemodel.h"
#include "processdocumentmanager.h"
// KDE
#include <KUrl>
#include <KLocale>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QTreeView>

#include <KDebug>


static const char DocumentProtocol[] = "documents";


ProcessDocumentConnectDialog::ProcessDocumentConnectDialog( QWidget* parent )
 : KDialog( parent )
{
    setCaption( i18nc("@title:window","Connect to") );
    setButtons( Ok | Cancel );
    setButtonGuiItem( Ok, KGuiItem(i18nc("@action:button","&Connect"), "network-connect",
                      i18nc("@info:tooltip","Connect to the document"),
                      i18nc("@info:whatsthis","If you press the <interface>Connect</interface> "
                            "button, the document you selected will be connected to.")) );
    setDefaultButton( Cancel );
    setModal( false );

    QWidget* page = new QWidget( this );
    setMainWidget( page );

    mAccessableDocumentTableModel = new ProcressDocumentTableModel( this );

    QVBoxLayout *baseLayout = new QVBoxLayout( page );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( KDialog::spacingHint() );

    mTableView = new QTreeView( page );
    mTableView->setObjectName( "AccessableDocumentTable" );
    mTableView->setRootIsDecorated( false );
    mTableView->setItemsExpandable( false );
    mTableView->setUniformRowHeights( true );
    mTableView->setAllColumnsShowFocus( true );
    mTableView->setModel( mAccessableDocumentTableModel );
    connect( mTableView->selectionModel(),
             SIGNAL(selectionChanged( const QItemSelection&, const QItemSelection& )),
             SLOT(onDocumentSelectionChanged()) );

    baseLayout->addWidget( mTableView, 10 );

    onDocumentSelectionChanged();
}

AbstractProcessDocument* ProcessDocumentConnectDialog::processDocument() const
{
    const int listIndex = mTableView->currentIndex().row();
    return mAccessableDocumentTableModel->document( listIndex );
}

KUrl ProcessDocumentConnectDialog::getUrl() const
{
    const QString id = processDocument()->id();

    KUrl result;
    result.setProtocol( DocumentProtocol );
    result.setFileName( id );
    return result;
}


void ProcessDocumentConnectDialog::onDocumentSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mTableView->selectionModel();
    const bool hasSelection = selectionModel->hasSelection();
    enableButtonOk( hasSelection );
}

ProcessDocumentConnectDialog::~ProcessDocumentConnectDialog() {}
