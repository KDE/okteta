/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelcodecmanager.hpp"
#include "modelcodecmanager_p.hpp"

namespace Kasten {

ModelCodecManager::ModelCodecManager()
    : d_ptr(new ModelCodecManagerPrivate())
{
}

ModelCodecManager::~ModelCodecManager() = default;

std::vector<AbstractModelStreamEncoder*>
ModelCodecManager::streamEncoders(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_D(const ModelCodecManager);

    return d->streamEncoders(model, selection);
}

QVector<AbstractModelStreamDecoder*>
ModelCodecManager::decoderList() const
{
    Q_D(const ModelCodecManager);

    return d->decoderList();
}

std::vector<AbstractModelDataGenerator*> ModelCodecManager::dataGenerators() const
{
    Q_D(const ModelCodecManager);

    return d->dataGenerators();
}

QVector<AbstractModelExporter*>
ModelCodecManager::exporterList(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_D(const ModelCodecManager);

    return d->exporterList(model, selection);
}

void ModelCodecManager::setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog)
{
    Q_D(ModelCodecManager);

    d->setOverwriteDialog(overwriteDialog);
}

void ModelCodecManager::setStreamEncoders(std::vector<std::unique_ptr<AbstractModelStreamEncoder>>&& streamEncoderList)
{
    Q_D(ModelCodecManager);

    d->setStreamEncoders(std::move(streamEncoderList));
}

void ModelCodecManager::setStreamDecoders(const QVector<AbstractModelStreamDecoder*>& decoderList)
{
    Q_D(ModelCodecManager);

    d->setStreamDecoders(decoderList);
}

void ModelCodecManager::setDataGenerators(std::vector<std::unique_ptr<AbstractModelDataGenerator>>&& dataGeneratorList)
{
    Q_D(ModelCodecManager);

    d->setDataGenerators(std::move(dataGeneratorList));
}

void ModelCodecManager::encodeToStream(AbstractModelStreamEncoder* encoder,
                                       AbstractModel* model, const AbstractModelSelection* selection)
{
    Q_D(ModelCodecManager);

    d->encodeToStream(encoder, model, selection);
}

void ModelCodecManager::exportDocument(AbstractModelExporter* exporter,
                                       AbstractModel* model, const AbstractModelSelection* selection)
{
    Q_D(ModelCodecManager);

    d->exportDocument(exporter, model, selection);
}

}

#include "moc_modelcodecmanager.cpp"
