/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelencoderfilesystemexporterconfigeditorfactory.hpp"

// lib
#include "modelencoderfilesystemexporterconfigeditor.hpp"
#include <abstractmodelstreamencoderconfigeditorfactory.hpp>
#include <abstractmodelstreamencoderconfigeditor.hpp>
// Kasten core
#include <Kasten/ModelEncoderFileSystemExporter>
// Std
#include <utility>

namespace Kasten {

ModelEncoderFileSystemExporterConfigEditorFactory::ModelEncoderFileSystemExporterConfigEditorFactory(AbstractModelStreamEncoderConfigEditorFactory* encoderConfigEditorFactory)
    : mEncoderConfigEditorFactory(encoderConfigEditorFactory)
{
}

ModelEncoderFileSystemExporterConfigEditorFactory::~ModelEncoderFileSystemExporterConfigEditorFactory()
{
    // TODO: do not delete mEncoderConfigEditorFactory here, is deleted outside, no clear control that way
}

std::unique_ptr<AbstractModelExporterConfigEditor> ModelEncoderFileSystemExporterConfigEditorFactory::tryCreateConfigEditor(AbstractModelExporter* exporter) const
{
    std::unique_ptr<AbstractModelExporterConfigEditor> result;

    auto* const modelEncoderFileSystemExporter = qobject_cast<ModelEncoderFileSystemExporter*>(exporter);

    if (modelEncoderFileSystemExporter) {
        auto encoderConfigEditor =
            mEncoderConfigEditorFactory->tryCreateConfigEditor(modelEncoderFileSystemExporter->encoder());

        if (encoderConfigEditor) {
            result = std::make_unique<ModelEncoderFileSystemExporterConfigEditor>(std::move(encoderConfigEditor));
        }
    }

    return result;
}

}
