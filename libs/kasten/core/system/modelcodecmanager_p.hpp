/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELCODECMANAGER_P_HPP
#define KASTEN_MODELCODECMANAGER_P_HPP

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
#if 0
    std::vector<AbstractModelStreamDecoder*> streamDecoders() const;
#endif
    std::vector<AbstractModelDataGenerator*> dataGenerators() const;

    std::vector<AbstractModelExporter*> exporters(AbstractModel* model, const AbstractModelSelection* selection) const;

public:
    void setStreamEncoders(std::vector<std::unique_ptr<AbstractModelStreamEncoder>>&& streamEncoderList);
#if 0
    void setStreamDecoders(std::vector<std::unique_ptr<AbstractModelStreamDecoder>>&& streamDecoderList);
#endif
    void setDataGenerators(std::vector<std::unique_ptr<AbstractModelDataGenerator>>&& dataGeneratorList);
    void setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog);

private:
    // used for dialogs, TODO: create (or use?) global instance for this
    AbstractOverwriteDialog* mOverwriteDialog = nullptr;

    // temporary hack: hard coded codecs for byte arrays
    std::vector<std::unique_ptr<AbstractModelStreamEncoder>> mStreamEncoderList;
#if 0
    std::vector<std::unique_ptr<AbstractModelStreamDecoder>> mStreamDecoderList;
#endif
    std::vector<std::unique_ptr<AbstractModelDataGenerator>> mDataGeneratorList;
    std::vector<std::unique_ptr<AbstractModelExporter>> mExporterList;
};



}

#endif
