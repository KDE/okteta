/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modelcodecmanager.hpp"

// lib
#include "modelencoderfilesystemexporter.hpp"
#include "abstractmodelstreamencoder.hpp"
// #include "abstractmodelstreamdecoder.hpp"
#include "abstractmodeldatagenerator.hpp"
#include "abstractoverwritedialog.hpp"
#include "jobmanager.hpp"
#include "documentmanager.hpp"
#include "abstractexportjob.hpp"
// KF5
#include <KIO/StatJob>
#include <KJobWidgets>
#include <KLocalizedString>
// Qt
#include <QFileDialog>
#include <QUrl>

namespace Kasten {

ModelCodecManager::ModelCodecManager(DocumentManager* manager)
    : mManager(manager)
{
}

ModelCodecManager::~ModelCodecManager()
{
    qDeleteAll(mExporterList);
    qDeleteAll(mEncoderList);
//     qDeleteAll( mDecoderList );
    qDeleteAll(mGeneratorList);
}

QList<AbstractModelStreamEncoder*>
ModelCodecManager::encoderList(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)
    return model ? mEncoderList : QList<AbstractModelStreamEncoder*>();
}

QVector<AbstractModelStreamDecoder*>
ModelCodecManager::decoderList() const { return mDecoderList; }

QVector<AbstractModelDataGenerator*>
ModelCodecManager::generatorList() const { return mGeneratorList; }

QList<AbstractModelExporter*>
ModelCodecManager::exporterList(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)
    return model ? mExporterList : QList<AbstractModelExporter*>();
}

void ModelCodecManager::setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog)
{
    mOverwriteDialog = overwriteDialog;
}

void ModelCodecManager::setEncoders(const QList<AbstractModelStreamEncoder*>& encoderList)
{
    mEncoderList = encoderList;

    qDeleteAll(mExporterList);
    mExporterList.clear();

    mExporterList.reserve(mEncoderList.size());
    for (AbstractModelStreamEncoder* encoder : qAsConst(mEncoderList)) {
        mExporterList << new ModelEncoderFileSystemExporter(encoder);
    }
}

void ModelCodecManager::setDecoders(const QVector<AbstractModelStreamDecoder*>& decoderList)
{
    mDecoderList = decoderList;
}

void ModelCodecManager::setGenerators(const QVector<AbstractModelDataGenerator*>& generatorList)
{
    mGeneratorList = generatorList;
}

void ModelCodecManager::encodeToStream(AbstractModelStreamEncoder* encoder,
                                       AbstractModel* model, const AbstractModelSelection* selection)
{
    Q_UNUSED(selection)
    Q_UNUSED(model)
    Q_UNUSED(encoder)
//    AbstractDocument* model = mFactory->create();
//    mManager->addDocument( model );
}

void ModelCodecManager::exportDocument(AbstractModelExporter* exporter,
                                       AbstractModel* model, const AbstractModelSelection* selection)
{
    bool exportDone = false;

    const QString dialogTitle =
        i18nc("@title:window", "Export");
    do {
        QFileDialog exportFileDialog(/*mWidget*/ nullptr, dialogTitle);

        exportFileDialog.setAcceptMode(QFileDialog::AcceptSave);
        exportFileDialog.setFileMode(QFileDialog::AnyFile);
        const QStringList mimeTypes = QStringList { exporter->remoteMimeType() };
        exportFileDialog.setMimeTypeFilters(mimeTypes);

        exportFileDialog.setLabelText(QFileDialog::Accept, i18nc("@action:button", "&Export"));

        exportFileDialog.exec();

        const QList<QUrl> exportUrls = exportFileDialog.selectedUrls();

        if (!exportUrls.isEmpty()) {
            const QUrl& exportUrl = exportUrls.at(0);

            KIO::StatJob* statJob = KIO::stat(exportUrl);
            statJob->setSide(KIO::StatJob::DestinationSide);
            KJobWidgets::setWindow(statJob, /*mWidget*/ nullptr);

            const bool isUrlInUse = statJob->exec();

            if (isUrlInUse) {
                // TODO: care for case that file from url is already loaded by (only?) this program
//                     const bool otherFileLoaded = mManager->documentByUrl( exportUrl );
                // TODO: replace "file" with synchronizer->storageTypeName() or such
                // TODO: offer "Synchronize" as alternative, if supported, see below
                const Answer answer =
                    mOverwriteDialog ? mOverwriteDialog->queryOverwrite(exportUrl, dialogTitle) : Cancel;
                if (answer == Cancel) {
                    break;
                }
                if (answer == PreviousQuestion) {
                    continue;
                }
            }

            AbstractExportJob* exportJob = exporter->startExport(model, selection, exportUrl);
            exportDone = JobManager::executeJob(exportJob);

//                 if( exportDone )
//                     emit urlUsed( exportUrl );
        } else {
            break;
        }
    } while (!exportDone);
}

}
