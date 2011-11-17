/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofilesmanagedialog.h"

// this
#include "viewprofileeditdialog.h"
#include "viewprofiletablemodel.h"
// KDE
#include <KPushButton>
#include <KLocale>
// Qt
#include <QtGui/QHBoxLayout>
#include <QtGui/QTreeView>

namespace Kasten1
{

ViewProfilesManageDialog::ViewProfilesManageDialog( QWidget* parent )
  : KDialog( parent )
{
    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QHBoxLayout* pageLayout = new QHBoxLayout( page );

    // profile list
    mViewProfileTableView = new QTreeView( page );
    mViewProfiles << ByteArrayViewProfile() << ByteArrayViewProfile();
    mViewProfileTableModel = new ViewProfileTableModel( &mViewProfiles, 1, this );
    mViewProfileTableView->setObjectName( QLatin1String( "ViewProfileTableView" ) );
    mViewProfileTableView->setHeaderHidden( true );
    mViewProfileTableView->setRootIsDecorated( false );
    mViewProfileTableView->setItemsExpandable( false );
    mViewProfileTableView->setUniformRowHeights( true );
    mViewProfileTableView->setAllColumnsShowFocus( true );
    mViewProfileTableView->setModel( mViewProfileTableModel );

    // buttons
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    KPushButton* createButton = // copy from selected
        new KPushButton( KGuiItem(i18nc("@action:button","&Create new..."), "document-new",
                      i18nc("@info:tooltip","Opens an editor for the view profile as default for all views."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Set as Default</interface> button, the view profile you selected "
                            "in the list is set as default for all views.")), page );
    connect( createButton, SIGNAL(clicked(bool)), SLOT(onCreateNewButtonClicked()) );
    buttonLayout->addWidget( createButton );
    KPushButton* editButton =
        new KPushButton( KGuiItem(i18nc("@action:button","&Edit..."), "document-edit",
                      i18nc("@info:tooltip","Opens an editor for the view profile as default for all views."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Set as Default</interface> button, the view profile you selected "
                            "in the list is set as default for all views.")), page );
    connect( editButton, SIGNAL(clicked(bool)), SLOT(onEditButtonClicked()) );
    buttonLayout->addWidget( editButton );
    KPushButton* setDefaultButton =
        new KPushButton( KGuiItem(i18nc("@action:button","&Set as Default"), QString(),
                      i18nc("@info:tooltip","Sets the selected view profile as default for all views."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Set as Default</interface> button, the view profile you selected "
                            "in the list is set as default for all views.")), page );
    connect( setDefaultButton, SIGNAL(clicked(bool)), SLOT(onSetDefaultButtonClicked()) );
    buttonLayout->addWidget( setDefaultButton );
    KPushButton* deleteButton =
        new KPushButton( KGuiItem(i18nc("@action:button","&Delete"), "list-remove",
                      i18nc("@info:tooltip","Deletes the selected view profile."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Delete</interface> button, the view profile you selected "
                            "in the list is deleted.")), page );
    connect( deleteButton, SIGNAL(clicked(bool)), SLOT(onDeleteButtonClicked()) );
    buttonLayout->addWidget( deleteButton );
    buttonLayout->addStretch();

    pageLayout->addWidget( mViewProfileTableView );
    pageLayout->addLayout( buttonLayout );

    setButtons( Close );
}

void ViewProfilesManageDialog::setViewProfiles( const QList<ByteArrayViewProfile>& viewProfiles )
{
    mViewProfiles = viewProfiles;
}

void ViewProfilesManageDialog::onCreateNewButtonClicked()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( this );

    const int answer = dialog->exec();
    if( answer == KDialog::Ok )
    {
    }

    delete dialog;
}

void ViewProfilesManageDialog::onEditButtonClicked()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( this );

    const int answer = dialog->exec();
    if( answer == KDialog::Ok )
    {
    }

    delete dialog;
}

void ViewProfilesManageDialog::onSetDefaultButtonClicked()
{
}

void ViewProfilesManageDialog::onDeleteButtonClicked()
{
}

ViewProfilesManageDialog::~ViewProfilesManageDialog()
{
}

}
