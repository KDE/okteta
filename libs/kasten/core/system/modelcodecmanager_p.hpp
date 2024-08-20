/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELCODECMANAGER_P_HPP
#define KASTEN_MODELCODECMANAGER_P_HPP

// Qt
#include <QVector>
// Std
#include <memory>
#include <vector>

namespace Kasten {

class AbstractModel;
class AbstractModelSelection;
class AbstractModelStreamEncoder;
class AbstractModelStreamDecoder;
class AbstractModelDataGenerator;
class AbstractModelExporter;
class AbstractOverwriteDialog;

class ModelCodecManagerPrivate
{
public:
    ModelCodecManagerPrivate();
    ModelCodecManagerPrivate(const ModelCodecManagerPrivate&) = delete;
    ~ModelCodecManagerPrivate();

public:
    ModelCodecManagerPrivate& operator=(const ModelCodecManagerPrivate&) = delete;

public:
    void encodeToStream(AbstractModelStreamEncoder* encoder,
                        AbstractModel* model, const AbstractModelSelection* selection);

    void exportDocument(AbstractModelExporter* exporter,
                        AbstractModel* model, const AbstractModelSelection* selection);

public:
    std::vector<AbstractModelStreamEncoder*> streamEncoders(AbstractModel* model, const AbstractModelSelection* selection) const;
    QVector<AbstractModelStreamDecoder*> decoderList() const;
    QVector<AbstractModelDataGenerator*> generatorList() const;

    QVector<AbstractModelExporter*> exporterList(AbstractModel* model, const AbstractModelSelection* selection) const;

public:
    void setStreamEncoders(std::vector<std::unique_ptr<AbstractModelStreamEncoder>>&& streamEncoderList);
    void setStreamDecoders(const QVector<AbstractModelStreamDecoder*>& decoderList);
    void setDataGenerators(const QVector<AbstractModelDataGenerator*>& generatorList);
    void setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog);

private:
    // used for dialogs, TODO: create (or use?) global instance for this
    AbstractOverwriteDialog* mOverwriteDialog = nullptr;

    // temporary hack: hard coded codecs for byte arrays
    std::vector<std::unique_ptr<AbstractModelStreamEncoder>> mStreamEncoderList;
    QVector<AbstractModelStreamDecoder*> mDecoderList;
    QVector<AbstractModelDataGenerator*> mGeneratorList;
    QVector<AbstractModelExporter*> mExporterList;
};



}

#endif
