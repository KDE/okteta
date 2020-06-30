/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelencoderfilesystemexporterconfigeditor.hpp"

// lib
#include <abstractmodelstreamencoderconfigeditor.hpp>
// Qt
#include <QLayout>

namespace Kasten {

ModelEncoderFileSystemExporterConfigEditor::ModelEncoderFileSystemExporterConfigEditor(AbstractModelStreamEncoderConfigEditor* encoderConfigEditor)
    : mEncoderConfigEditor(encoderConfigEditor)
{
    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->addWidget(mEncoderConfigEditor);
}

ModelEncoderFileSystemExporterConfigEditor::~ModelEncoderFileSystemExporterConfigEditor()
{
    delete mEncoderConfigEditor;
}

bool ModelEncoderFileSystemExporterConfigEditor::isValid() const
{
    return mEncoderConfigEditor->isValid();
}

AbstractSelectionView* ModelEncoderFileSystemExporterConfigEditor::createPreviewView() const
{
    return mEncoderConfigEditor->createPreviewView();
}

}
