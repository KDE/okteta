/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELCODECVIEWMANAGER_P_HPP
#define KASTEN_MODELCODECVIEWMANAGER_P_HPP

// lib
#include "modelcodecviewmanager.hpp"

namespace Kasten {

class AbstractModelStreamEncoderConfigEditorFactory;
class AbstractModelStreamEncoderConfigEditor;
class AbstractModelStreamEncoder;
class AbstractModelExporterConfigEditorFactory;
class AbstractModelExporterConfigEditor;
class AbstractModelExporter;
class AbstractModelDataGeneratorConfigEditorFactory;
class AbstractModelDataGeneratorConfigEditor;
class AbstractModelDataGenerator;

class ModelCodecViewManagerPrivate
{
public:
    ModelCodecViewManagerPrivate();
    ModelCodecViewManagerPrivate(const ModelCodecViewManagerPrivate&) = delete;

    ~ModelCodecViewManagerPrivate();

public:
    ModelCodecViewManagerPrivate& operator=(const ModelCodecViewManagerPrivate&) = delete;

public:
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> createConfigEditor(AbstractModelStreamEncoder* encoder) const;
    std::unique_ptr<AbstractModelExporterConfigEditor> createConfigEditor(AbstractModelExporter* exporter) const;
    std::unique_ptr<AbstractModelDataGeneratorConfigEditor> createConfigEditor(AbstractModelDataGenerator* generator) const;

public:
    void setStreamEncoderConfigEditorFactories(std::vector<std::unique_ptr<AbstractModelStreamEncoderConfigEditorFactory>>&& factoryList);
    void setDataGeneratorConfigEditorFactories(std::vector<std::unique_ptr<AbstractModelDataGeneratorConfigEditorFactory>>&& factoryList);

private:
    std::vector<std::unique_ptr<AbstractModelStreamEncoderConfigEditorFactory>> mEncoderFactoryList;
    std::vector<std::unique_ptr<AbstractModelExporterConfigEditorFactory>> mExporterFactoryList;
    std::vector<std::unique_ptr<AbstractModelDataGeneratorConfigEditorFactory>> mGeneratorFactoryList;
};

}

#endif
