/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modelcodecviewmanager.hpp"
#include "modelcodecviewmanager_p.hpp"


namespace Kasten {

ModelCodecViewManager::ModelCodecViewManager()
    : d_ptr(new ModelCodecViewManagerPrivate())
{
}

ModelCodecViewManager::~ModelCodecViewManager() = default;

void ModelCodecViewManager::setEncoderConfigEditorFactories(const QVector<AbstractModelStreamEncoderConfigEditorFactory*>& factoryList)
{
    Q_D(ModelCodecViewManager);

    d->setEncoderConfigEditorFactories(factoryList);
}

void ModelCodecViewManager::setExporterConfigEditorFactories(const QVector<AbstractModelExporterConfigEditorFactory*>& factoryList)
{
    Q_D(ModelCodecViewManager);

    d->setExporterConfigEditorFactories(factoryList);
}

void ModelCodecViewManager::setGeneratorConfigEditorFactories(const QVector<AbstractModelDataGeneratorConfigEditorFactory*>& factoryList)
{
    Q_D(ModelCodecViewManager);

    d->setGeneratorConfigEditorFactories(factoryList);
}

AbstractModelStreamEncoderConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    Q_D(const ModelCodecViewManager);

    return d->createConfigEditor(encoder);
}

AbstractModelExporterConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelExporter* exporter) const
{
    Q_D(const ModelCodecViewManager);

    return d->createConfigEditor(exporter);
}

AbstractModelDataGeneratorConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelDataGenerator* generator) const
{
    Q_D(const ModelCodecViewManager);

    return d->createConfigEditor(generator);
}

}
