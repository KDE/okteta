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
#include <QVector>

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
    QVector<AbstractModelStreamEncoder*> encoderList(AbstractModel* model, const AbstractModelSelection* selection) const;
    QVector<AbstractModelStreamDecoder*> decoderList() const;
    QVector<AbstractModelDataGenerator*> generatorList() const;

    QVector<AbstractModelExporter*> exporterList(AbstractModel* model, const AbstractModelSelection* selection) const;

public:
    void setEncoders(const QVector<AbstractModelStreamEncoder*>& encoderList);
    void setDecoders(const QVector<AbstractModelStreamDecoder*>& decoderList);
    void setGenerators(const QVector<AbstractModelDataGenerator*>& generatorList);
    void setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog);

private:
    const QScopedPointer<class ModelCodecManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ModelCodecManager)
};

}

#endif
