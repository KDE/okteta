/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELCODECMANAGER_P_HPP
#define KASTEN_MODELCODECMANAGER_P_HPP

// Qt
#include <QVector>

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
    ModelCodecManagerPrivate() = default;
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
    // used for dialogs, TODO: create (or use?) global instance for this
    AbstractOverwriteDialog* mOverwriteDialog = nullptr;

    // temporary hack: hard coded codecs for byte arrays
    QVector<AbstractModelStreamEncoder*> mEncoderList;
    QVector<AbstractModelStreamDecoder*> mDecoderList;
    QVector<AbstractModelDataGenerator*> mGeneratorList;
    QVector<AbstractModelExporter*> mExporterList;
};



}

#endif
