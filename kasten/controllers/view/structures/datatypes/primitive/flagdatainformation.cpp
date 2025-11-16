/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "flagdatainformation.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QVarLengthArray>
// Std
#include <utility>

FlagDataInformation::FlagDataInformation(const QString& name,
                                         std::unique_ptr<PrimitiveDataInformation>&& type,
                                         const std::shared_ptr<EnumDefinition>& enumDef, DataInformation* parent)
    : EnumDataInformation(name, std::move(type), enumDef, parent)
{
    Q_ASSERT_X(mValue->type() != PrimitiveDataType::Double && mValue->type() != PrimitiveDataType::Float && mValue->type() != PrimitiveDataType::Invalid,
               "FlagDataInformation::FlagDataInformation", "Bitflags only work with integers!");
}

struct FlagData
{
    QString name;
    quint64 value;
};

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
    const std::map<AllPrimitiveTypes, QString>& values = mEnum->values();
    // I doubt more than 10 flags will be set very often -> only then do we need a malloc
    QVarLengthArray<FlagData, 10> arr;
    const auto value = mValue->value().value<quint64>();
    for (const auto& [enumeralValue, enumeralName] : values) {
        const auto flag = enumeralValue.value<quint64>();
        if ((value & flag) == flag) {
            // flag is set
            arr.append(FlagData{enumeralName, flag});
        }
    }

    // now we have all flags, check if some overlap
    for (int i = 0; i < arr.size(); ++i) {
        const quint64 firstFlag = arr.at(i).value;
        for (int j = 0; j < arr.size();) {
            if (j == i) {
                j++;
                continue;
            }
            // check if they overlap
            quint64 secondFlag = arr.at(j).value;
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
        result += arr.at(i).name;
        usedBits |= arr.at(i).value;
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
