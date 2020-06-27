/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filterjob.hpp"

// filter
#include <abstractbytearrayfilter.hpp>
// Qt
#include <QCoreApplication>

namespace Kasten {

bool FilterJob::exec()
{
    // TODO: what kind of signal could a filter send?
    connect(mByteArrayFilter, &AbstractByteArrayFilter::filteredBytes, this, &FilterJob::onFilteredBytes);

    const bool result = mByteArrayFilter->filter(mResult, mModel, mRange);

    deleteLater();

    return result;
}

void FilterJob::onFilteredBytes()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
}

}
