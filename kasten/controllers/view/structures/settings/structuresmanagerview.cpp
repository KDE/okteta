/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 Copyright 2009 Alex Richardson <alex.richardson@gmx.de>

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
#include <KIO/NetAccess>
#include <KMessageBox>
#include <KPluginSelector>
#include <KPluginInfo>
#include <KPushButton>
//KNS
#include <KNS3/KNewStuffButton>
// Qt
#include <QtGui/QListWidgetItem>
#include <QtGui/QLayout>
#include <QtGui/QSizePolicy>

#include <KDebug>

static const int FileNameRole = Qt::UserRole;

StructuresManagerView::StructuresManagerView(Kasten2::StructuresManager* manager,
        QWidget* parent) :
    QWidget(parent), mManager(manager), mRebuildingPluginsList(false)
{
    QVBoxLayout* pageLayout = new QVBoxLayout(this);

    mStructuresSelector = new KPluginSelector(this);
    connect(mStructuresSelector, SIGNAL(changed(bool)),
            SLOT(onPluginSelectorChange(bool)));
    pageLayout->addWidget(mStructuresSelector);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    //    mApplyChangesButton = new KPushButton(KIcon("dialog-ok-apply"), i18n(
    //            "Apply Changes"), this);
    //    connect(mApplyChangesButton, SIGNAL(clicked()), SLOT(onApplyChangesClicked()));
    //    buttonsLayout->addWidget(mApplyChangesButton, 0, Qt::AlignCenter);

    mGetNewStructuresButton = new KNS3::Button(i18n("Get New Structures..."),
            QLatin1String("okteta-structures.knsrc"), this);
    connect(mGetNewStructuresButton,
            SIGNAL(dialogFinished(KNS3::Entry::List)),
            SLOT(onGetNewStructuresClicked(KNS3::Entry::List)));

    buttonsLayout->addWidget(mGetNewStructuresButton);

    //    mUpdateStructuresButton = new KPushButton(KIcon("system-software-update"), i18n(
    //            "Check for updates"), this);
    //    connect(mGetNewStructuresButton, SIGNAL(clicked()),
    //            SLOT(onUpdateStructuresClicked()));
    //
    //    buttonsLayout->addWidget(mUpdateStructuresButton);

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

    //    buttonsLayout->addStretch();

    pageLayout->addLayout(buttonsLayout);

    rebuildPluginSelectorEntries();
    setLayout(pageLayout);
}

void StructuresManagerView::onGetNewStructuresClicked(
        const KNS3::Entry::List& changedEntries)
{
    foreach (const KNS3::Entry& e, changedEntries)
        {
            kDebug()
                << "Changed Entry: " << e.name();
            if (e.status() == KNS3::Entry::Installed)
            {
                //new element installed
                kDebug()
                    << "installed files:" << e.installedFiles();
            }
            if (e.status() == KNS3::Entry::Deleted)
            {
                //new element installed
                kDebug()
                    << "deleted files:" << e.uninstalledFiles();
            }
        }
    if (!changedEntries.isEmpty())
    {
        kDebug()
            << "installed structures changed ->"
                " rebuilding list of installed structures";
        mManager->reloadPaths();
        rebuildPluginSelectorEntries();
    }
}

void StructuresManagerView::onPluginSelectorChange(bool change)
{
    if (mRebuildingPluginsList)
        return;
    kDebug()
        << "pluginselector changed: " << change;
    if (!change)
        return;
    mStructuresSelector->save();
    emit selectedPluginsChanged();
}
#if 0
void StructuresManagerView::onUpdateStructuresClicked()
{
    //XXX: implement
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
#endif

void StructuresManagerView::rebuildPluginSelectorEntries()
{
    mRebuildingPluginsList = true;
    KPluginInfo::List plugins;
    KPluginInfo::List dynamicPlugins;
    foreach(const Kasten2::StructureDefinitionFile* def, mManager->structureDefs())
        {
            KPluginInfo info = def->pluginInfo();
            if (info.category() == QLatin1String("structure"))
                plugins.append(info);
            else if (info.category() == QLatin1String("structure/js"))
                dynamicPlugins.append(info);
        }

    //XXX is there any way to clear the plugins selector?
    layout()->removeWidget(mStructuresSelector);
    layout()->removeWidget(mGetNewStructuresButton);
    delete mStructuresSelector;
    mStructuresSelector = new KPluginSelector(this);
    connect(mStructuresSelector, SIGNAL(changed(bool)),
            SLOT(onPluginSelectorChange(bool)));
    layout()->addWidget(mStructuresSelector);
    layout()->addWidget(mGetNewStructuresButton);


    mStructuresSelector->addPlugins(plugins, KPluginSelector::ReadConfigFile, i18n(
            "Structure Definitions"), QLatin1String("structure"), mManager->config());
    mStructuresSelector->addPlugins(dynamicPlugins, KPluginSelector::ReadConfigFile,
            i18n("Dynamic Structure Definitions"), QLatin1String("structure/js"),
            mManager->config());
    mStructuresSelector->load();
    mStructuresSelector->updatePluginsState();
    mRebuildingPluginsList = false;
}

StructuresManagerView::~StructuresManagerView()
{
}
