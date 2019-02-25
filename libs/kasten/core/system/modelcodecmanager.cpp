/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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
