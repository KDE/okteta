/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELCODECMANAGER_HPP
#define KASTEN_MODELCODECMANAGER_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>
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

class ModelCodecManagerPrivate;

class KASTENCORE_EXPORT ModelCodecManager : public QObject
{
    Q_OBJECT

public:
    explicit ModelCodecManager();
    ~ModelCodecManager() override;

public:
    // or use the viewmodel here? on what should the export be based?
    void encodeToStream(AbstractModelStreamEncoder* encoder,
                        AbstractModel* model, const AbstractModelSelection* selection);

    void exportDocument(AbstractModelExporter* exporter,
                        AbstractModel* model, const AbstractModelSelection* selection);

public:
    [[nodiscard]]
    std::vector<AbstractModelStreamEncoder*> streamEncoders(AbstractModel* model, const AbstractModelSelection* selection) const;
#if 0
    std::vector<AbstractModelStreamDecoder*> streamDecoders() const;
#endif
    [[nodiscard]]
    std::vector<AbstractModelDataGenerator*> dataGenerators() const;

    [[nodiscard]]
    std::vector<AbstractModelExporter*> exporters(AbstractModel* model, const AbstractModelSelection* selection) const;

public:
    void setStreamEncoders(std::vector<std::unique_ptr<AbstractModelStreamEncoder>>&& streamEncoderList);
#if 0
    void setStreamDecoders(std::vector<std::unique_ptr<AbstractModelStreamDecoder>>&& streamDecoderList);
#endif
    void setDataGenerators(std::vector<std::unique_ptr<AbstractModelDataGenerator>>&& dataGeneratorList);
    void setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog);

private:
    const std::unique_ptr<class ModelCodecManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ModelCodecManager)
};

}

#endif
