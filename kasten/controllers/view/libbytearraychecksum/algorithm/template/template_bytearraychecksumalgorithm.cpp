/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearraychecksumalgorithm.hpp" to the name of the header of your checksum algorithm,
//// e.g. "mybytearraychecksumalgorithm.hpp"
#include "template_bytearraychecksumalgorithm.hpp"
//// ADAPT(end)

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

Template_ByteArrayChecksumAlgorithm::Template_ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
//// ADAPT(start)
//// change "TEMPLATE" to a short and descriptive name of the checksum algorithm
        i18nc("name of the checksum algorithm", "Template"),
//// change "TEMPLATE" to a unique id of the checksum algorithm
        QStringLiteral("TEMPLATE"))
//// ADAPT(end)
{}

Template_ByteArrayChecksumAlgorithm::~Template_ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* Template_ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Template_ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                            const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    bool success = true;

//// ADAPT(start)
//// modify the following code to calculate the checksum/hashsum.
//// The final checksum is passed as a QString to result.
    const int mask = (1 << mParameterSet.bitNumber());

    int sum = 0;

    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        sum ^= (model->byte(i) & mask);

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            Q_EMIT calculatedBytes(range.localIndex(i) + 1);
        }
    }

    *result = QStringLiteral("%1").arg(sum);
//// ADAPT(end)

    return success;
}

#include "moc_template_bytearraychecksumalgorithm.cpp"
