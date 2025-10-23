/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYSTREAMENCODER_HPP
#define KASTEN_ABSTRACTBYTEARRAYSTREAMENCODER_HPP

// lib
#include "oktetakastengui_export.hpp"
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

private:
    static constexpr Okteta::Address MaxPreviewSize = 100;

public:
    AbstractByteArrayStreamEncoder(const QString& remoteTypeName, const QString& remoteMimeType,
                                   const QString& remoteClipboardMimeType = QString());
    ~AbstractByteArrayStreamEncoder() override;

public: // AbstractModelStreamEncoder API
    [[nodiscard]]
    bool encodeToStream(QIODevice* device, AbstractModel* model, const AbstractModelSelection* selection) override;
    [[nodiscard]]
    QString modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const override;

public:
    [[nodiscard]]
    QByteArray previewData(AbstractModel* model, const AbstractModelSelection* selection);

Q_SIGNALS:
    void settingsChanged();

protected: // API to be implemented
    [[nodiscard]]
    virtual bool encodeDataToStream(QIODevice* device,
                                    const ByteArrayView* byteArrayView,
                                    const Okteta::AbstractByteArrayModel* byteArrayModel,
                                    const Okteta::AddressRange& range) = 0;
};

}

#endif
