/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007,2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_ABSTRACTMODELSTREAMENCODER_HPP
#define KASTEN_ABSTRACTMODELSTREAMENCODER_HPP

// lib
#include <kasten/kastencore_export.hpp>
// Qt
#include <QObject>
#include <QString>

class QIODevice;

namespace Kasten {

class AbstractModel;
class AbstractModelSelection;

class AbstractModelStreamEncoderPrivate;

// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile

// TODO: perhaps just create one instance per encode process,
// giving AbstractModel* model, AbstractModelSelection* selection in the constructor?
// so all precalculated values like used model (type) can be cached
class KASTENCORE_EXPORT AbstractModelStreamEncoder : public QObject
{
    Q_OBJECT

protected:
    AbstractModelStreamEncoder(AbstractModelStreamEncoderPrivate* d);

public:
    AbstractModelStreamEncoder(const QString& remoteTypeName, const QString& remoteMimeType,
                               const QString& remoteClipboardMimeType = QString());

    ~AbstractModelStreamEncoder() override;

public: // API to be implemented
    virtual bool encodeToStream(QIODevice* device, AbstractModel* model, const AbstractModelSelection* selection) = 0;
    virtual QString modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const = 0;

public:
    QString remoteTypeName() const;
    QString remoteMimeType() const;
    // the clipboard does not yet understand mimetype inheritance
    QString remoteClipboardMimeType() const;

protected:
    const QScopedPointer<AbstractModelStreamEncoderPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractModelStreamEncoder)
};

}

#endif
