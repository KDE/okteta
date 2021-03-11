/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELCODECVIEWMANAGER_HPP
#define KASTEN_MODELCODECVIEWMANAGER_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QObject>
#include <QVector>

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

class ModelCodecViewManagerPrivate;

class KASTENGUI_EXPORT ModelCodecViewManager : public QObject
{
    Q_OBJECT

public:
    ModelCodecViewManager();
    ~ModelCodecViewManager() override;

public:
    AbstractModelStreamEncoderConfigEditor* createConfigEditor(AbstractModelStreamEncoder* encoder) const;
    AbstractModelExporterConfigEditor* createConfigEditor(AbstractModelExporter* exporter) const;
    AbstractModelDataGeneratorConfigEditor* createConfigEditor(AbstractModelDataGenerator* generator) const;

public:
    void setEncoderConfigEditorFactories(const QVector<AbstractModelStreamEncoderConfigEditorFactory*>& factoryList);
    void setExporterConfigEditorFactories(const QVector<AbstractModelExporterConfigEditorFactory*>& factoryList);
    void setGeneratorConfigEditorFactories(const QVector<AbstractModelDataGeneratorConfigEditorFactory*>& factoryList);

private:
    const QScopedPointer<class ModelCodecViewManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ModelCodecViewManager)
};

}

#endif
