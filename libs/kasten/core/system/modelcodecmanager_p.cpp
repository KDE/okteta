/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelcodecmanager_p.hpp"

// lib
#include "modelencoderfilesystemexporter.hpp"
#include "abstractmodelstreamencoder.hpp"
// #include "abstractmodelstreamdecoder.hpp"
#include "abstractmodeldatagenerator.hpp"
#include "abstractoverwritedialog.hpp"
#include "jobmanager.hpp"
#include "abstractexportjob.hpp"
// KF
#include <KIO/StatJob>
#include <KJobWidgets>
#include <KLocalizedString>
// Qt
#include <QFileDialog>
#include <QUrl>

namespace Kasten {

ModelCodecManagerPrivate::~ModelCodecManagerPrivate()
{
    qDeleteAll(mExporterList);
    qDeleteAll(mEncoderList);
//     qDeleteAll( mDecoderList );
    qDeleteAll(mGeneratorList);
}

QVector<AbstractModelStreamEncoder*>
ModelCodecManagerPrivate::encoderList(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)
    return model ? mEncoderList : QVector<AbstractModelStreamEncoder*>();
}

QVector<AbstractModelStreamDecoder*>
ModelCodecManagerPrivate::decoderList() const { return mDecoderList; }

QVector<AbstractModelDataGenerator*>
ModelCodecManagerPrivate::generatorList() const { return mGeneratorList; }

QVector<AbstractModelExporter*>
ModelCodecManagerPrivate::exporterList(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)
    return model ? mExporterList : QVector<AbstractModelExporter*>();
}

void ModelCodecManagerPrivate::setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog)
{
    mOverwriteDialog = overwriteDialog;
}

void ModelCodecManagerPrivate::setEncoders(const QVector<AbstractModelStreamEncoder*>& encoderList)
{
    mEncoderList = encoderList;

    qDeleteAll(mExporterList);
    mExporterList.clear();

    mExporterList.reserve(mEncoderList.size());
    for (AbstractModelStreamEncoder* encoder : qAsConst(mEncoderList)) {
        mExporterList << new ModelEncoderFileSystemExporter(encoder);
    }
}

void ModelCodecManagerPrivate::setDecoders(const QVector<AbstractModelStreamDecoder*>& decoderList)
{
    mDecoderList = decoderList;
}

void ModelCodecManagerPrivate::setGenerators(const QVector<AbstractModelDataGenerator*>& generatorList)
{
    mGeneratorList = generatorList;
}

void ModelCodecManagerPrivate::encodeToStream(AbstractModelStreamEncoder* encoder,
                                       AbstractModel* model, const AbstractModelSelection* selection)
{
    Q_UNUSED(selection)
    Q_UNUSED(model)
    Q_UNUSED(encoder)
//    AbstractDocument* model = mFactory->create();
//    mManager->addDocument( model );
}

void ModelCodecManagerPrivate::exportDocument(AbstractModelExporter* exporter,
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
