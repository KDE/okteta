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
    AbstractModelStreamEncoderConfigEditor* createConfigEditor(AbstractModelStreamEncoder* encoder) const;
    AbstractModelExporterConfigEditor* createConfigEditor(AbstractModelExporter* exporter) const;
    AbstractModelDataGeneratorConfigEditor* createConfigEditor(AbstractModelDataGenerator* generator) const;

public:
    void setEncoderConfigEditorFactories(const QVector<AbstractModelStreamEncoderConfigEditorFactory*>& factoryList);
    void setExporterConfigEditorFactories(const QVector<AbstractModelExporterConfigEditorFactory*>& factoryList);
    void setGeneratorConfigEditorFactories(const QVector<AbstractModelDataGeneratorConfigEditorFactory*>& factoryList);

private:
    QVector<AbstractModelStreamEncoderConfigEditorFactory*> mEncoderFactoryList;
    QVector<AbstractModelExporterConfigEditorFactory*> mExporterFactoryList;
    QVector<AbstractModelDataGeneratorConfigEditorFactory*> mGeneratorFactoryList;
};

}

#endif
