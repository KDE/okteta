/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHANGE_HPP
#define OKTETA_BYTEARRAYCHANGE_HPP

// lib
#include "oktetacore_export.hpp"
#include "arraychangemetrics.hpp"
// Qt
#include <QByteArray>

namespace Okteta {

// TODO: do we need the invalid status?
// TODO: what about grouped changes
// TODO: use change names from original? Only if local are not available
class OKTETACORE_EXPORT ByteArrayChange
{
    friend QDataStream& operator<<(QDataStream& outStream, const ByteArrayChange& change);
    friend QDataStream& operator>>(QDataStream& inStream, ByteArrayChange& change);

public:
    ByteArrayChange();
    explicit ByteArrayChange(const ArrayChangeMetrics& metrics, const QByteArray& data = QByteArray());

public:
    const ArrayChangeMetrics& metrics() const;
    const QByteArray& data() const;

private:
    ArrayChangeMetrics mMetrics;
    QByteArray mData;
//     UserId mUserId;
//      mTime;
};

inline ByteArrayChange::ByteArrayChange() = default;

inline ByteArrayChange::ByteArrayChange(const ArrayChangeMetrics& metrics, const QByteArray& data)
    : mMetrics(metrics)
    , mData(data)
{}

inline const ArrayChangeMetrics& ByteArrayChange::metrics() const { return mMetrics; }
inline const QByteArray& ByteArrayChange::data()            const { return mData; }

QDataStream& operator<<(QDataStream& outStream, const ByteArrayChange& change);
QDataStream& operator>>(QDataStream& inStream, ByteArrayChange& change);

inline QDataStream& operator<<(QDataStream& outStream, const ByteArrayChange& change)
{
    outStream << change.mMetrics << change.mData;
    return outStream;
}

inline QDataStream& operator>>(QDataStream& inStream, ByteArrayChange& change)
{
    inStream >> change.mMetrics >> change.mData;
    return inStream;
}

}

Q_DECLARE_TYPEINFO(Okteta::ByteArrayChange, Q_MOVABLE_TYPE);

#endif
