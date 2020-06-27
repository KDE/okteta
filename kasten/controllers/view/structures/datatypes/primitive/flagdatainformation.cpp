/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "flagdatainformation.hpp"
#include <QVarLengthArray>
#include <KLocalizedString>

FlagDataInformation::FlagDataInformation(const QString& name, PrimitiveDataInformation* type,
                                         const EnumDefinition::Ptr& enumDef, DataInformation* parent)
    : EnumDataInformation(name, type, enumDef, parent)
{
    Q_ASSERT_X(type->type() != PrimitiveDataType::Double && type->type() != PrimitiveDataType::Float && type->type() != PrimitiveDataType::Invalid,
               "FlagDataInformation::FlagDataInformation", "Bitflags only work with integers!");
}

using FlagPair = QPair<QString, quint64>;

template <typename T, int len>
static void removeFromArray(QVarLengthArray<T, len>& array, int index)
{
    Q_ASSERT(index >= 0 && index < array.size());
    int max = array.size() - 1;
    for (int i = index; i < max; ++i) {
        array[i] = array[i + 1];
    }

    array.removeLast();
}

QString FlagDataInformation::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    QMapIterator<AllPrimitiveTypes, QString> iter(mEnum->values());
    // I doubt more than 10 flags will be set very often -> only then do we need a malloc
    QVarLengthArray<FlagPair, 10> arr;
    const quint64 value = mValue->value().value<quint64>();
    while (iter.hasNext()) {
        iter.next();
        const quint64 flag = iter.key().value<quint64>();
        if ((value & flag) == flag) {
            // flag is set
            arr.append(qMakePair(iter.value(), flag));
        }
    }

    // now we have all flags, check if some overlap
    for (int i = 0; i < arr.size(); ++i) {
        const quint64 firstFlag = arr.at(i).second;
        for (int j = 0; j < arr.size();) {
            if (j == i) {
                j++;
                continue;
            }
            // check if they overlap
            quint64 secondFlag = arr.at(j).second;
            if ((firstFlag & secondFlag) == secondFlag) {
                // they overlap, remove the second flag
                removeFromArray(arr, j);
                if (j < i) {
                    i--; // i was pushed back by one as well
                }
            } else {
                j++;
            }
        }
    }

    // if array has zero elements just return the value in hexadecimal
    if (arr.isEmpty()) {
        return i18n("0x%1 (no matching flags)", QString::number(value, 16));
    }

    // make sure all we also show remaining bits at the end
    quint64 usedBits = 0;
    QString result;
    for (int i = 0, size = arr.size(); i < size; ++i) {
        if (i != 0) {
            result += QLatin1String(" | ");
        }
        result += arr.at(i).first;
        usedBits |= arr.at(i).second;
    }

    // TODO remove a NONE flag if others present (value = 0)

    // TODO set invalid if value not completely covered by flags

    if (usedBits != value) {
        quint64 missing = value & ~usedBits;
        result += QLatin1String(" | 0x") + QString::number(missing, 16);
    }

    return result;
}

QString FlagDataInformation::typeNameImpl() const
{
    return i18nc("Displayed in the type column. first comes the name "
                 "of the enum, then the underlying type (e.g. uint32)",
                 "flag %1 (%2)", mEnum->name(), mValue->typeName());
}
