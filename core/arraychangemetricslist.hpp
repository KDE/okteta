/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ARRAYCHANGEMETRICSLIST_HPP
#define OKTETA_ARRAYCHANGEMETRICSLIST_HPP

// lib
#include "arraychangemetrics.hpp"
//
#include <QVector>

namespace Okteta {

// TODO: do we need the invalid status?
class ArrayChangeMetricsList : public QVector<ArrayChangeMetrics>
{
public:
    static ArrayChangeMetricsList oneReplacement(Address offset, Size removeLength, Size insertLength);
    static ArrayChangeMetricsList oneSwapping(Address firstOffset, Size secondOffset, Size secondLength);

public:
    ArrayChangeMetricsList();
    explicit ArrayChangeMetricsList(const ArrayChangeMetrics& metrics);
};

inline ArrayChangeMetricsList ArrayChangeMetricsList::oneReplacement(Address offset, Size removeLength, Size insertLength)
{
    return ArrayChangeMetricsList(ArrayChangeMetrics::asReplacement(offset, removeLength, insertLength));
}

inline ArrayChangeMetricsList ArrayChangeMetricsList::oneSwapping(Address firstOffset, Size secondOffset, Size secondLength)
{
    return ArrayChangeMetricsList(ArrayChangeMetrics::asSwapping(firstOffset, secondOffset, secondLength));
}

inline ArrayChangeMetricsList::ArrayChangeMetricsList() = default;
inline ArrayChangeMetricsList::ArrayChangeMetricsList(const ArrayChangeMetrics& metrics)
{
    append(metrics);
}

}

#endif
