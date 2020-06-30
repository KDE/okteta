/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILTERJOB_HPP
#define KASTEN_FILTERJOB_HPP

// Okteta core
#include <Okteta/AddressRange>
#include <Okteta/Byte>
// Qt
#include <QObject>

class AbstractByteArrayFilter;

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class FilterJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    FilterJob(AbstractByteArrayFilter* byteArrayFilter,
              Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range);

public:
    bool exec();

private Q_SLOTS:
    void onFilteredBytes();

private:
    AbstractByteArrayFilter* mByteArrayFilter;

    Okteta::Byte* mResult;
    Okteta::AbstractByteArrayModel* mModel;
    const Okteta::AddressRange mRange;
};

inline FilterJob::FilterJob(AbstractByteArrayFilter* byteArrayFilter,
                            Okteta::Byte* result, Okteta::AbstractByteArrayModel* model,
                            const Okteta::AddressRange& range)
    : mByteArrayFilter(byteArrayFilter)
    , mResult(result)
    , mModel(model)
    , mRange(range)
{}

}

#endif
