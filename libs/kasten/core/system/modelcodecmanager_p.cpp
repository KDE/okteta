/*
    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelcodecmanager_p.hpp"

// lib
#include "modelencoderfilesystemexporter.hpp"
#include "abstractmodelstreamencoder.hpp"
#include "abstractmodeldatagenerator.hpp"
#include "abstractoverwritedialog.hpp"
#include "jobmanager.hpp"
#include "abstractexportjob.hpp"
// KF
#include <KIO/StatJob>
#include <KJobWidgets>
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QFileDialog>
#include <QPointer>
#include <QUrl>
// Std
#include <utility>

namespace Kasten {

namespace ModelCodecManagerNS {

// TODO: is there a way/pattern to reuse the original unique_ptr_vector memory?
template<typename T>
auto make_vector(const std::vector<std::unique_ptr<T>>& unique_ptr_vector)
{
    std::vector<T*> vector;
    vector.reserve(unique_ptr_vector.size());
    for (const auto& unique_ptr : unique_ptr_vector) {
        vector.emplace_back(unique_ptr.get());
    }
    return vector;
}

}

ModelCodecManagerPrivate::ModelCodecManagerPrivate() = default;

ModelCodecManagerPrivate::~ModelCodecManagerPrivate() = default;

std::vector<AbstractModelStreamEncoder*>
ModelCodecManagerPrivate::streamEncoders(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)
    return model ? ModelCodecManagerNS::make_vector(mStreamEncoderList) : std::vector<AbstractModelStreamEncoder*>();
}

#if 0
std::vector<AbstractModelStreamDecoder*>
ModelCodecManagerPrivate::streamDecoders() const
{
    return ModelCodecManagerNS::make_vector(mStreamDecoderList);
}
#endif

std::vector<AbstractModelDataGenerator*>
ModelCodecManagerPrivate::dataGenerators() const
{
    return ModelCodecManagerNS::make_vector(mDataGeneratorList);

}

std::vector<AbstractModelExporter*>
ModelCodecManagerPrivate::exporters(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)
    return model ? ModelCodecManagerNS::make_vector(mExporterList) : std::vector<AbstractModelExporter*>();
}

void ModelCodecManagerPrivate::setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog)
{
    mOverwriteDialog = overwriteDialog;
}

void ModelCodecManagerPrivate::setStreamEncoders(std::vector<std::unique_ptr<AbstractModelStreamEncoder>>&& streamStreamEncoderList)
{
    mExporterList.clear();

    mStreamEncoderList = std::move(streamStreamEncoderList);

    mExporterList.reserve(mStreamEncoderList.size());
    for (const auto& encoder : mStreamEncoderList) {
        mExporterList.emplace_back(std::make_unique<ModelEncoderFileSystemExporter>(encoder.get()));
    }
}

#if 0
void ModelCodecManagerPrivate::setStreamDecoders(std::vector<std::unique_ptr<AbstractModelStreamDecoder>>&& streamDecoderList)
{
    mStreamDecoderList = std::move(streamDecoderList);
}
#endif

void ModelCodecManagerPrivate::setDataGenerators(std::vector<std::unique_ptr<AbstractModelDataGenerator>>&& dataGeneratorList)
{
    mDataGeneratorList = std::move(dataGeneratorList);
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
        QPointer<QFileDialog> exportFileDialog = new QFileDialog(QApplication::activeWindow(), dialogTitle);

        exportFileDialog->setAcceptMode(QFileDialog::AcceptSave);
        exportFileDialog->setOption(QFileDialog::DontConfirmOverwrite);
        exportFileDialog->setFileMode(QFileDialog::AnyFile);
        const QStringList mimeTypes = QStringList { exporter->remoteMimeType() };
        exportFileDialog->setMimeTypeFilters(mimeTypes);

        exportFileDialog->setLabelText(QFileDialog::Accept, i18nc("@action:button", "&Export"));

        if (exportFileDialog->exec() == QDialog::Rejected) {
            break;
        }
        if (!exportFileDialog) {
            break;
        }

        const QList<QUrl> exportUrls = exportFileDialog->selectedUrls();

        delete exportFileDialog;

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
//                     Q_EMIT urlUsed( exportUrl );
        } else {
            break;
        }
    } while (!exportDone);
}

}
