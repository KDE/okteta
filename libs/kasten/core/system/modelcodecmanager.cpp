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

QVector<AbstractModelStreamEncoder*>
ModelCodecManager::encoderList(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_D(const ModelCodecManager);

    return d->encoderList(model, selection);
}

QVector<AbstractModelStreamDecoder*>
ModelCodecManager::decoderList() const
{
    Q_D(const ModelCodecManager);

    return d->decoderList();
}

QVector<AbstractModelDataGenerator*>
ModelCodecManager::generatorList() const
{
    Q_D(const ModelCodecManager);

    return d->generatorList();
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

void ModelCodecManager::setEncoders(const QVector<AbstractModelStreamEncoder*>& encoderList)
{
    Q_D(ModelCodecManager);

    d->setEncoders(encoderList);
}

void ModelCodecManager::setDecoders(const QVector<AbstractModelStreamDecoder*>& decoderList)
{
    Q_D(ModelCodecManager);

    d->setDecoders(decoderList);
}

void ModelCodecManager::setGenerators(const QVector<AbstractModelDataGenerator*>& generatorList)
{
    Q_D(ModelCodecManager);

    d->setGenerators(generatorList);
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
