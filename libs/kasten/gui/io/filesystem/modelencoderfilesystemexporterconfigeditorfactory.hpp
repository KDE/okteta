/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELENCODERFILESYSTEMEXPORTERCONFIGEDITORFACTORY_HPP
#define KASTEN_MODELENCODERFILESYSTEMEXPORTERCONFIGEDITORFACTORY_HPP

// lib
#include "abstractmodelexporterconfigeditorfactory.hpp"

namespace Kasten {

class AbstractModelStreamEncoderConfigEditorFactory;

class KASTENGUI_EXPORT ModelEncoderFileSystemExporterConfigEditorFactory : public AbstractModelExporterConfigEditorFactory
{
public:
    explicit ModelEncoderFileSystemExporterConfigEditorFactory(AbstractModelStreamEncoderConfigEditorFactory* encoderConfigEditorFactory);
    ~ModelEncoderFileSystemExporterConfigEditorFactory() override;

public:
    AbstractModelExporterConfigEditor* tryCreateConfigEditor(AbstractModelExporter* exporter) const override;

private:
    AbstractModelStreamEncoderConfigEditorFactory* mEncoderConfigEditorFactory;
};

}

#endif
