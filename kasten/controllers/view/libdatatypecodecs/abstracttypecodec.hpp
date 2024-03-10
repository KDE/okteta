/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTTYPECODEC_HPP
#define OKTETA_ABSTRACTTYPECODEC_HPP

// Qt
#include <QString>
#include <QVariant>

class QByteArray;

namespace Okteta {
class PODData;

class AbstractTypeCodec
{
protected:
    explicit AbstractTypeCodec(const QString& name);

public:
    AbstractTypeCodec(const AbstractTypeCodec&) = delete;

    virtual ~AbstractTypeCodec();

    AbstractTypeCodec& operator=(const AbstractTypeCodec&) = delete;

public: // API to be implemented
    virtual QVariant value(const PODData& data, int* byteCount) const = 0;
    virtual QByteArray valueToBytes(const QVariant& value) const = 0;

    virtual bool areEqual(const QVariant& value, QVariant& otherValue) const = 0;

public:
    const QString& name() const;

private:
    QString mName;
};

inline AbstractTypeCodec::AbstractTypeCodec(const QString& name) : mName(name) {}
inline AbstractTypeCodec::~AbstractTypeCodec() = default;

inline const QString& AbstractTypeCodec::name() const { return mName; }

}

#endif
