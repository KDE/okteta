/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelcodecviewmanager.hpp"
#include "modelcodecviewmanager_p.hpp"

// lib
#include <abstractmodelexporterconfigeditor.hpp>
#include <abstractmodeldatageneratorconfigeditor.hpp>

namespace Kasten {

ModelCodecViewManager::ModelCodecViewManager()
    : d_ptr(std::make_unique<ModelCodecViewManagerPrivate>())
{
}

ModelCodecViewManager::~ModelCodecViewManager() = default;

void ModelCodecViewManager::setStreamEncoderConfigEditorFactories(std::vector<std::unique_ptr<AbstractModelStreamEncoderConfigEditorFactory>>&& factoryList)
{
    Q_D(ModelCodecViewManager);

    d->setStreamEncoderConfigEditorFactories(std::move(factoryList));
}

void ModelCodecViewManager::setDataGeneratorConfigEditorFactories(std::vector<std::unique_ptr<AbstractModelDataGeneratorConfigEditorFactory>>&& factoryList)
{
    Q_D(ModelCodecViewManager);

    d->setDataGeneratorConfigEditorFactories(std::move(factoryList));
}

AbstractModelStreamEncoderConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    Q_D(const ModelCodecViewManager);

    return d->createConfigEditor(encoder);
}

std::unique_ptr<AbstractModelExporterConfigEditor> ModelCodecViewManager::createConfigEditor(AbstractModelExporter* exporter) const
{
    Q_D(const ModelCodecViewManager);

    return d->createConfigEditor(exporter);
}

std::unique_ptr<AbstractModelDataGeneratorConfigEditor> ModelCodecViewManager::createConfigEditor(AbstractModelDataGenerator* generator) const
{
    Q_D(const ModelCodecViewManager);

    return d->createConfigEditor(generator);
}

}

#include "moc_modelcodecviewmanager.cpp"
