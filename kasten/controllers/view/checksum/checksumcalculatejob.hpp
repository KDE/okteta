/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHECKSUMCALCULATEJOB_HPP
#define KASTEN_CHECKSUMCALCULATEJOB_HPP

// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QObject>

class AbstractByteArrayChecksumAlgorithm;

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class ChecksumCalculateJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    ChecksumCalculateJob(QString* checksum,
                         const AbstractByteArrayChecksumAlgorithm* algorithm,
                         const Okteta::AbstractByteArrayModel* model,
                         const Okteta::AddressRange& selection);

public:
    void exec();

private Q_SLOTS:
    void onCalculatedBytes();

private:
    QString* mChecksum;

    const AbstractByteArrayChecksumAlgorithm* mAlgorithm;

    const Okteta::AbstractByteArrayModel* mByteArrayModel;
    const Okteta::AddressRange mSelection;
};

inline ChecksumCalculateJob::ChecksumCalculateJob(QString* checksum,
                                                  const AbstractByteArrayChecksumAlgorithm* algorithm,
                                                  const Okteta::AbstractByteArrayModel* model,
                                                  const Okteta::AddressRange& selection)
    : mChecksum(checksum)
    , mAlgorithm(algorithm)
    , mByteArrayModel(model)
    , mSelection(selection)
{}

}

#endif
