/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELSTREAMENCODER_HPP
#define KASTEN_ABSTRACTMODELSTREAMENCODER_HPP

// lib
#include "kastencore_export.hpp"
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
