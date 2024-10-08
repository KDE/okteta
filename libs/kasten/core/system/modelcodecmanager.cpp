/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelcodecmanager.hpp"
#include "modelcodecmanager_p.hpp"

namespace Kasten {

ModelCodecManager::ModelCodecManager()
    : d_ptr(std::make_unique<ModelCodecManagerPrivate>())
{
}

ModelCodecManager::~ModelCodecManager() = default;

std::vector<AbstractModelStreamEncoder*>
ModelCodecManager::streamEncoders(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_D(const ModelCodecManager);

    return d->streamEncoders(model, selection);
}

#if 0
std::vector<AbstractModelStreamDecoder*>
ModelCodecManager::streamDecoders() const
{
    Q_D(const ModelCodecManager);

    return d->streamDecoders();
}
#endif

std::vector<AbstractModelDataGenerator*> ModelCodecManager::dataGenerators() const
{
    Q_D(const ModelCodecManager);

    return d->dataGenerators();
}

std::vector<AbstractModelExporter*>
ModelCodecManager::exporters(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_D(const ModelCodecManager);

    return d->exporters(model, selection);
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

#if 0
void ModelCodecManager::setStreamDecoders(std::vector<std::unique_ptr<AbstractModelStreamDecoder>>&& streamDecoderList)
{
    Q_D(ModelCodecManager);

    d->setStreamDecoders(std::move(streamDecoderList));
}
#endif

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
