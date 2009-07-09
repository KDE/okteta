/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modelcodecmanager.h"

// lib
#include "modelencoderfilesystemexporter.h"
#include "abstractmodelstreamencoder.h"
// #include "abstractmodelstreamdecoder.h"
#include "jobmanager.h"
#include "kdocumentmanager.h"
#include "abstractexportjob.h"
// KDE
#include <KIO/NetAccess>
#include <KFileDialog>
#include <KMessageBox>
#include <KLocale>


namespace Kasten
{

ModelCodecManager::ModelCodecManager( KDocumentManager *manager )
 : mManager( manager ), mWidget( 0 ) {}

QList<AbstractModelStreamEncoder*>
ModelCodecManager::encoderList( AbstractModel *model, const AbstractModelSelection *selection ) const
{
Q_UNUSED( selection )
    return model ? mEncoderList : QList<AbstractModelStreamEncoder*>();
}

QList<AbstractModelStreamDecoder*>
ModelCodecManager::decoderList() const { return mDecoderList; }

QList<AbstractModelExporter*>
ModelCodecManager::exporterList( AbstractModel *model, const AbstractModelSelection *selection ) const
{
Q_UNUSED( selection )
    return model ? mExporterList : QList<AbstractModelExporter*>();
}

void ModelCodecManager::setWidget( QWidget *widget )
{
    mWidget = widget;
}

void ModelCodecManager::setEncoders( QList<AbstractModelStreamEncoder*> &encoderList )
{
    mEncoderList = encoderList;

    qDeleteAll( mExporterList );
    mExporterList.clear();

    foreach( AbstractModelStreamEncoder *encoder, mEncoderList )
        mExporterList << new ModelEncoderFileSystemExporter( encoder );
}

void ModelCodecManager::setDecoders( QList<AbstractModelStreamDecoder*> &decoderList )
{
    mDecoderList = decoderList;
}

void ModelCodecManager::encodeToStream( AbstractModelStreamEncoder *encoder,
                                        AbstractModel *model, const AbstractModelSelection *selection )
{
   Q_UNUSED( selection )
   Q_UNUSED( model )
   Q_UNUSED( encoder )
//    KAbstractDocument *model = mFactory->create();
//    mManager->addDocument( model );
}


void ModelCodecManager::exportDocument( AbstractModelExporter *exporter,
                                        AbstractModel *model, const AbstractModelSelection *selection )
{
    bool exportDone = false;

    const QString dialogTitle =
        i18nc( "@title:window", "Export" );
    do
    {
        KUrl exportUrl = KFileDialog::getSaveUrl( /*mWorkingUrl.url()*/QString(), QString(), mWidget, dialogTitle );

        if( !exportUrl.isEmpty() )
        {
            const bool isUrlInUse = KIO::NetAccess::exists( exportUrl, KIO::NetAccess::DestinationSide, mWidget );

            if( isUrlInUse )
            {
                // TODO: care for case that file from url is already loaded by (only?) this program
//                     const bool otherFileLoaded = mManager->documentByUrl( exportUrl );
                // TODO: replace "file" with synchronizer->storageTypeName() or such
                // TODO: offer "Synchronize" as alternative, if supported, see below
                const QString message =
                    i18nc( "@info",
                            "There is already a file at<nl/><filename>%1</filename>.<nl/>"
                            "Overwrite?", exportUrl.url() );
                const int answer = KMessageBox::warningYesNoCancel( mWidget, message, dialogTitle,
                                                                    KStandardGuiItem::overwrite(),
                                                                    KStandardGuiItem::back() );
                if( answer == KMessageBox::Cancel )
                    break;
                if( answer == KMessageBox::No )
                    continue;
            }

            AbstractExportJob *exportJob = exporter->startExport( model, selection, exportUrl );
            exportDone = JobManager::executeJob( exportJob, mWidget );

//                 if( exportDone )
//                     emit urlUsed( exportUrl );
        }
        else
            break;
    }
    while( !exportDone );
}

ModelCodecManager::~ModelCodecManager()
{
    qDeleteAll( mExporterList );
    qDeleteAll( mEncoderList );
//     qDeleteAll( mDecoderList );
}

}
