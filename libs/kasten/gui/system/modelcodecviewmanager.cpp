/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

ModelCodecViewManager::ModelCodecViewManager() = default;

ModelCodecViewManager::~ModelCodecViewManager()
{
    qDeleteAll(mEncoderFactoryList);
    qDeleteAll(mExporterFactoryList);
    qDeleteAll(mGeneratorFactoryList);
}

void ModelCodecViewManager::setEncoderConfigEditorFactories(const QList<AbstractModelStreamEncoderConfigEditorFactory*>& factoryList)
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

void ModelCodecViewManager::setExporterConfigEditorFactories(const QList<AbstractModelExporterConfigEditorFactory*>& factoryList)
{
    qDeleteAll(mExporterFactoryList);
    mExporterFactoryList = factoryList;
}

void ModelCodecViewManager::setGeneratorConfigEditorFactories(const QList<AbstractModelDataGeneratorConfigEditorFactory*>& factoryList)
{
    qDeleteAll(mGeneratorFactoryList);
    mGeneratorFactoryList = factoryList;
}

AbstractModelStreamEncoderConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelStreamEncoder* encoder) const
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

AbstractModelExporterConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelExporter* exporter) const
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

AbstractModelDataGeneratorConfigEditor* ModelCodecViewManager::createConfigEditor(AbstractModelDataGenerator* generator) const
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
