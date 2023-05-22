/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSEARCHJOB_HPP
#define KASTEN_BYTEARRAYSEARCHJOB_HPP

// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QObject>
#include <QByteArray>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}

namespace Kasten {

class ByteArraySearchJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    ByteArraySearchJob(const Okteta::AbstractByteArrayModel* model,
                       const QByteArray& searchData,
                       Okteta::Address startIndex, Okteta::Address endIndex,
                       Qt::CaseSensitivity caseSensitivity, const QString& charCodecName);

    ~ByteArraySearchJob() override;

public:
    Okteta::AddressRange exec();
    void start();

Q_SIGNALS:
    void finished(Okteta::AddressRange position);

private Q_SLOTS:
    void onBytesSearched();

private:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;

    QByteArray mSearchData;
    Okteta::Address mStartIndex;
    Okteta::Address mEndIndex;
    Qt::CaseSensitivity mCaseSensitivity;
    const Okteta::CharCodec* mCharCodec;

    Okteta::AddressRange m_result;
};

}

#endif
