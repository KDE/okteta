/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelcodecviewmanager_p.hpp"

// lib
#include <filesystem/modelencoderfilesystemexporterconfigeditorfactory.hpp>
#include <abstractmodelstreamencoderconfigeditorfactory.hpp>
#include <abstractmodelstreamencoderconfigeditor.hpp>
#include <abstractmodelexporterconfigeditorfactory.hpp>
#include <abstractmodelexporterconfigeditor.hpp>
#include <abstractmodeldatageneratorconfigeditorfactory.hpp>
#include <abstractmodeldatageneratorconfigeditor.hpp>
// Kasten core
#include <Kasten/AbstractModelExporter>
#include <Kasten/AbstractModelDataGenerator>
#include <Kasten/AbstractModelStreamEncoder>
// Std
#include <utility>

namespace Kasten {

ModelCodecViewManagerPrivate::ModelCodecViewManagerPrivate() = default;

ModelCodecViewManagerPrivate::~ModelCodecViewManagerPrivate() = default;

void ModelCodecViewManagerPrivate::setStreamEncoderConfigEditorFactories(std::vector<std::unique_ptr<AbstractModelStreamEncoderConfigEditorFactory>>&& factoryList)
{
    mExporterFactoryList.clear();

    mEncoderFactoryList = std::move(factoryList);

    mExporterFactoryList.reserve(mEncoderFactoryList.size());
    for (const auto& factory : mEncoderFactoryList) {
        mExporterFactoryList.emplace_back(std::make_unique<ModelEncoderFileSystemExporterConfigEditorFactory>(factory.get()));
    }
}

void ModelCodecViewManagerPrivate::setDataGeneratorConfigEditorFactories(std::vector<std::unique_ptr<AbstractModelDataGeneratorConfigEditorFactory>>&& factoryList)
{
    mGeneratorFactoryList = std::move(factoryList);
}

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ModelCodecViewManagerPrivate::createConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;

    for (const auto& factory : mEncoderFactoryList) {
        result = factory->tryCreateConfigEditor(encoder);
        if (result) {
            break;
        }
    }

    return result;
}

std::unique_ptr<AbstractModelExporterConfigEditor> ModelCodecViewManagerPrivate::createConfigEditor(AbstractModelExporter* exporter) const
{
    std::unique_ptr<AbstractModelExporterConfigEditor> result;

    for (const auto& factory : mExporterFactoryList) {
        result = factory->tryCreateConfigEditor(exporter);
        if (result) {
            break;
        }
    }

    return result;
}

std::unique_ptr<AbstractModelDataGeneratorConfigEditor> ModelCodecViewManagerPrivate::createConfigEditor(AbstractModelDataGenerator* generator) const
{
    std::unique_ptr<AbstractModelDataGeneratorConfigEditor> result;

    for (const auto& factory : mGeneratorFactoryList) {
        result = factory->tryCreateConfigEditor(generator);
        if (result) {
            break;
        }
    }

    return result;
}

}
