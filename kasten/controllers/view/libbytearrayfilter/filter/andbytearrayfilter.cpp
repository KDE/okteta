/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "andbytearrayfilter.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KLocalizedString>

static constexpr char AndFilterConfigGroupId[] = "AND";

AndByteArrayFilter::AndByteArrayFilter()
    : AbstractByteArrayFilter(
        i18nc("name of the filter; it does a logic AND operation",
              "operand AND data"),
        QStringLiteral("AND")
      )
{}

AndByteArrayFilter::~AndByteArrayFilter() = default;

AbstractByteArrayFilterParameterSet* AndByteArrayFilter::parameterSet() { return &mParameterSet; }

void AndByteArrayFilter::loadConfig(const KConfigGroup& configGroup)
{
    const KConfigGroup filterConfigGroup = configGroup.group(AndFilterConfigGroupId);

    mParameterSet.loadConfig(filterConfigGroup);
}

void AndByteArrayFilter::saveConfig(KConfigGroup& configGroup) const
{
    KConfigGroup filterConfigGroup = configGroup.group(AndFilterConfigGroupId);

    mParameterSet.saveConfig(filterConfigGroup);
}

bool AndByteArrayFilter::filter(Okteta::Byte* result,
                                Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    const QByteArray operand = mParameterSet.operand();
    const int operandSize = operand.size();

    if (mParameterSet.alignAtEnd()) {
        const int behindLastResult = range.width();
        int r = behindLastResult;
        Okteta::Address m = range.nextBehindEnd();
        int nextBlockEnd = r - FilteredByteCountSignalLimit;

        while (m > range.start()) {
            int o = operandSize;
            while (m > range.start() && o > 0) {
                result[(r--) - 1] = model->byte((m--) - 1) & operand[(o--) - 1];
            }

            if (r <= nextBlockEnd) {
                nextBlockEnd -= FilteredByteCountSignalLimit;
                Q_EMIT filteredBytes(behindLastResult - r);
            }
        }
    } else {
        int r = 0;
        Okteta::Address m = range.start();
        int nextBlockEnd = FilteredByteCountSignalLimit;

        while (m <= range.end()) {
            int o = 0;
            while (m <= range.end() && o < operandSize) {
                result[r++] = model->byte(m++) & operand[o++];
            }

            if (r >= nextBlockEnd) {
                nextBlockEnd += FilteredByteCountSignalLimit;
                Q_EMIT filteredBytes(r);
            }
        }
    }

    return true;
}

#include "moc_andbytearrayfilter.cpp"
