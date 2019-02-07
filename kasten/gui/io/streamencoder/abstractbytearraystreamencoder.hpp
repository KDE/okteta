/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_ABSTRACTBYTEARRAYSTREAMENCODER_HPP
#define KASTEN_ABSTRACTBYTEARRAYSTREAMENCODER_HPP

// lib
#include <kasten/okteta/oktetakastengui_export.hpp>
// Kasten core
#include <Kasten/AbstractModelStreamEncoder>
// Okteta core
#include <Okteta/AddressRange>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

// TODO: make display model an interface, so these encoders can stay in core
class ByteArrayView;

class OKTETAKASTENGUI_EXPORT AbstractByteArrayStreamEncoder : public AbstractModelStreamEncoder
{
    Q_OBJECT

public:
    AbstractByteArrayStreamEncoder(const QString& remoteTypeName, const QString& remoteMimeType);
    ~AbstractByteArrayStreamEncoder() override;

public: // AbstractModelStreamEncoder API
    bool encodeToStream(QIODevice* device, AbstractModel* model, const AbstractModelSelection* selection) override;
    QString modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const override;

public:
    QString previewData(AbstractModel* model, const AbstractModelSelection* selection);

Q_SIGNALS:
    void settingsChanged();

protected: // API to be implemented
    virtual bool encodeDataToStream(QIODevice* device,
                                    const ByteArrayView* byteArrayView,
                                    const Okteta::AbstractByteArrayModel* byteArrayModel,
                                    const Okteta::AddressRange& range) = 0;
};

}

#endif
