/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelencoderfilesystemexporterconfigeditorfactory.hpp"

// lib
#include "modelencoderfilesystemexporterconfigeditor.hpp"
#include <abstractmodelstreamencoderconfigeditorfactory.hpp>
// Kasten core
#include <Kasten/ModelEncoderFileSystemExporter>

namespace Kasten {

ModelEncoderFileSystemExporterConfigEditorFactory::ModelEncoderFileSystemExporterConfigEditorFactory(AbstractModelStreamEncoderConfigEditorFactory* encoderConfigEditorFactory)
    : mEncoderConfigEditorFactory(encoderConfigEditorFactory)
{
}

ModelEncoderFileSystemExporterConfigEditorFactory::~ModelEncoderFileSystemExporterConfigEditorFactory()
{
    // TODO: do not delete mEncoderConfigEditorFactory here, is deleted outside, no clear control that way
}

AbstractModelExporterConfigEditor* ModelEncoderFileSystemExporterConfigEditorFactory::tryCreateConfigEditor(AbstractModelExporter* exporter) const
{
    AbstractModelExporterConfigEditor* result = nullptr;

    auto* modelEncoderFileSystemExporter = qobject_cast<ModelEncoderFileSystemExporter*>(exporter);

    if (modelEncoderFileSystemExporter) {
        AbstractModelStreamEncoderConfigEditor* encoderConfigEditor =
            mEncoderConfigEditorFactory->tryCreateConfigEditor(modelEncoderFileSystemExporter->encoder());

        if (encoderConfigEditor) {
            result = new ModelEncoderFileSystemExporterConfigEditor(encoderConfigEditor);
        }
    }

    return result;
}

}
