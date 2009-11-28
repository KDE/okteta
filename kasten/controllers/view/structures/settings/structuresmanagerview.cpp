/*
 This file is part of the Okteta Kasten module, part of the KDE project.

 Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "structuresmanagerview.h"

#include "structviewpreferences.h"
#include "../structuresmanager.h"
// KDE
#include <KFileDialog>
#include <KStandardDirs>
#include <KNS/Engine>
#include <KIO/NetAccess>
#include <KMessageBox>
#include <KPluginSelector>
#include <KPluginInfo>
#include <KPushButton>
// Qt
#include <QtGui/QListWidgetItem>
#include <QtGui/QLayout>

#include <KDebug>

static const int FileNameRole = Qt::UserRole;

StructuresManagerView::StructuresManagerView( Kasten::StructuresManager* manager,
        QWidget* parent)
  : QWidget(parent),
    mManager(manager)
{
    QVBoxLayout* pageLayout = new QVBoxLayout( this );

    mStructuresSelector = new KPluginSelector( this );
    pageLayout->addWidget( mStructuresSelector );

    QVBoxLayout* buttonsLayout = new QVBoxLayout();

//    mGetNewStructuresButton = new KPushButton( KIcon("get-hot-new-stuff"), i18n("Get New Structures..."), this );
//    connect( mGetNewStructuresButton, SIGNAL(clicked()),
//             SLOT(onGetNewStructuresClicked()) );
//
//    buttonsLayout->addWidget( mGetNewStructuresButton );
//
//    mImportStructuresButton = new KPushButton( i18n("Import Structures..."), this );
//    connect( mImportStructuresButton, SIGNAL(clicked()),
//             SLOT(onImportStructuresClicked()) );
//
//    buttonsLayout->addWidget( mImportStructuresButton );
//
//    mExportStructureButton = new KPushButton( i18n("Export Structure..."), this );
//    connect( mExportStructureButton, SIGNAL(clicked()),
//             SLOT(onExportStructureClicked()) );
//
//    buttonsLayout->addWidget( mExportStructureButton );
//
//    mRemoveStructureButton = new KPushButton( i18n("Remove Structure"), this );
//    connect( mRemoveStructureButton, SIGNAL(clicked()),
//             SLOT(onRemoveStructureClicked()) );
//
//    buttonsLayout->addWidget( mRemoveStructureButton );

    mApplyChangesButton = new KPushButton( i18n("Apply Changes"), this );
    connect( mApplyChangesButton, SIGNAL(clicked()),
             SLOT(onApplyChangesClicked()) );

    buttonsLayout->addWidget( mApplyChangesButton );
//    buttonsLayout->addStretch();

    pageLayout->addLayout( buttonsLayout );


    KPluginInfo::List plugins;
    foreach( const Kasten::StructureDefinitionFile* def, manager->structureDefs() )
        plugins.append( def->info() );

    mStructuresSelector->addPlugins(plugins, KPluginSelector::ReadConfigFile, i18n(
            "Structure Definitions"), QString("structure"), mManager->config());
    mStructuresSelector->load();
}

void StructuresManagerView::onGetNewStructuresClicked()
{
    KNS::Engine engine(this);
    if (engine.init("structures.knsrc"))
    {
        const KNS::Entry::List entries = engine.downloadDialogModal(this);

        if (!entries.isEmpty())
        {
            mManager->reloadPaths();
        }
    }
}

void StructuresManagerView::onImportStructuresClicked()
{
    const KUrl::List urls = KFileDialog::getOpenUrls(KUrl(), i18n(
            "*.osd|Okteta structure definition files (*.osd)"), this, i18nc(
            "@title:window Do import the structure definitions",
            "Import Structure Definitions"));
    foreach( const KUrl& url, urls )
        {
            // TODO: use Structure Manager
            // TODO: possibly untar or uncompress it
            // open it

            // load the scheme
            //         load url.path();
            //         if okay
            //             save
        }
}

void StructuresManagerView::onExportStructureClicked()
{
    return;
//     TODO Not working ATM
//        if (currentItem != 0)
       {
           const KUrl saveUrl = KFileDialog::getSaveUrl(KUrl(), i18n(
                   "*.osd|Okteta structure definition files (*.osd)"), this, i18nc(
                   "@title:window Do export the structure definition",
                   "Export Structure Definition"));

           if (!saveUrl.isEmpty())
           {
               const QString fileName;// = currentItem->data(FileNameRole).toString();
               const QString filePath = KGlobal::dirs()->findResource("data",
                       "okteta/structures/" + fileName + ".osd");
               kDebug() << fileName << filePath << saveUrl;
               const bool success = KIO::NetAccess::upload(filePath, saveUrl, this);

               if (!success)
                   KMessageBox::error(this, KIO::NetAccess::lastErrorString(), i18n(
                           "Error"));
           }
       }
}
void StructuresManagerView::onApplyChangesClicked()
{
    mStructuresSelector->save();
    kDebug() << "saved";
    emit applyButtonClicked();
    return;
}
void StructuresManagerView::onRemoveStructureClicked()
{
    mStructuresSelector->save();
    kDebug() << "saved";
    return;
//     QListWidgetItem* currentItem = ui.structuresView->currentItem();
//     if (currentItem != 0)
    {
        const QString fileName;// = currentItem->data(FileNameRole).toString();
        const QString filePath = KGlobal::dirs()->findResource("data",
                "okteta/structures/" + fileName + ".osd");
        const bool success = KIO::NetAccess::del(filePath, this); // TODO: why netaccess?
        if (success)
            //             delete schemeList->takeItem( schemeList->currentRow() );
            ;
        else
            KMessageBox::error(
                    this,
                    i18n(
                            "You do not have permission to delete that structure definition."),
                    i18n("Error"));
    }
}

StructuresManagerView::~StructuresManagerView()
{
}
