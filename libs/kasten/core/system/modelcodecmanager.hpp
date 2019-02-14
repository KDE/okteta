/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_MODELCODECMANAGER_HPP
#define KASTEN_MODELCODECMANAGER_HPP

// lib
#include <kasten/kastencore_export.hpp>
// Qt
#include <QObject>
#include <QVector>

class QString;

namespace Kasten {

class AbstractModel;
class AbstractModelSelection;
class AbstractModelStreamEncoder;
class AbstractModelStreamDecoder;
class AbstractModelDataGenerator;
class AbstractModelExporter;
class AbstractOverwriteDialog;

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
