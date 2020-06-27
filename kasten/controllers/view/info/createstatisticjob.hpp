/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CREATESTATISTICJOB_HPP
#define KASTEN_CREATESTATISTICJOB_HPP

// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QObject>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class CreateStatisticJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    CreateStatisticJob(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& selection,
                       int* byteCount);

public:
    // return size of selected byte array
    int exec();

private:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;
    const Okteta::AddressRange mSelection;

    int* const mByteCount;
};

inline CreateStatisticJob::CreateStatisticJob(const Okteta::AbstractByteArrayModel* model,
                                              const Okteta::AddressRange& selection,
                                              int* byteCount)
    : mByteArrayModel(model)
    , mSelection(selection)
    , mByteCount(byteCount)
{}

}

#endif
