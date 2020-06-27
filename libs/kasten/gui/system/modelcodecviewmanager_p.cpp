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

namespace Kasten {

ModelCodecViewManagerPrivate::ModelCodecViewManagerPrivate() = default;

ModelCodecViewManagerPrivate::~ModelCodecViewManagerPrivate()
{
    qDeleteAll(mEncoderFactoryList);
    qDeleteAll(mExporterFactoryList);
    qDeleteAll(mGeneratorFactoryList);
}

void ModelCodecViewManagerPrivate::setEncoderConfigEditorFactories(const QVector<AbstractModelStreamEncoderConfigEditorFactory*>& factoryList)
{
    qDeleteAll(mEncoderFactoryList);
    mEncoderFactoryList = factoryList;

    qDeleteAll(mExporterFactoryList);
    mExporterFactoryList.clear();

    mExporterFactoryList.reserve(mEncoderFactoryList.size());
    for (AbstractModelStreamEncoderConfigEditorFactory* factory : qAsConst(mEncoderFactoryList)) {
        mExporterFactoryList << new ModelEncoderFileSystemExporterConfigEditorFactory(factory);
    }
}

void ModelCodecViewManagerPrivate::setExporterConfigEditorFactories(const QVector<AbstractModelExporterConfigEditorFactory*>& factoryList)
{
    qDeleteAll(mExporterFactoryList);
    mExporterFactoryList = factoryList;
}

void ModelCodecViewManagerPrivate::setGeneratorConfigEditorFactories(const QVector<AbstractModelDataGeneratorConfigEditorFactory*>& factoryList)
{
    qDeleteAll(mGeneratorFactoryList);
    mGeneratorFactoryList = factoryList;
}

AbstractModelStreamEncoderConfigEditor* ModelCodecViewManagerPrivate::createConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    AbstractModelStreamEncoderConfigEditor* result = nullptr;

    for (const AbstractModelStreamEncoderConfigEditorFactory* factory : mEncoderFactoryList) {
        result = factory->tryCreateConfigEditor(encoder);
        if (result) {
            break;
        }
    }

    return result;
}

AbstractModelExporterConfigEditor* ModelCodecViewManagerPrivate::createConfigEditor(AbstractModelExporter* exporter) const
{
    AbstractModelExporterConfigEditor* result = nullptr;

    for (const AbstractModelExporterConfigEditorFactory* factory : mExporterFactoryList) {
        result = factory->tryCreateConfigEditor(exporter);
        if (result) {
            break;
        }
    }

    return result;
}

AbstractModelDataGeneratorConfigEditor* ModelCodecViewManagerPrivate::createConfigEditor(AbstractModelDataGenerator* generator) const
{
    AbstractModelDataGeneratorConfigEditor* result = nullptr;

    for (const AbstractModelDataGeneratorConfigEditorFactory* factory : mGeneratorFactoryList) {
        result = factory->tryCreateConfigEditor(generator);
        if (result) {
            break;
        }
    }

    return result;
}

}
