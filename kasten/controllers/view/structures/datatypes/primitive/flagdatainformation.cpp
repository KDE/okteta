/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011  Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */


#include "flagdatainformation.h"
#include "structviewpreferences.h"
#include <QVarLengthArray>

FlagDataInformation::FlagDataInformation(QString name, PrimitiveDataInformation* type,
        EnumDefinition::Ptr enumDef, DataInformation* parent)
    : EnumDataInformation(name, type, enumDef, parent)
{
    Q_ASSERT_X(type->type() != Type_Double && type->type() != Type_Float && type->type() != Type_NotPrimitive,
        "FlagDataInformation::FlagDataInformation", "Bitflags only work with integers!");
}

typedef QPair<QString, quint64> FlagPair;

template<typename T, int len>
static void removeFromArray(QVarLengthArray<T, len>& array, int index) {
    Q_ASSERT(index >= 0 && index < array.size());
    int max = array.size() - 1;
    for (int i = index; i < max; ++i) {
        array[i] = array[i + 1];
    }
    array.removeLast();
}

QString FlagDataInformation::valueString() const
{
    QMapIterator<AllPrimitiveTypes, QString> iter(mEnum->values());
    //I doubt more than 10 flags will be set very often -> only then do we need a malloc
    QVarLengthArray<FlagPair, 10> arr;
    const quint64 value = mValue->value().ulongValue;
    while(iter.hasNext()) {
        iter.next();
        const quint64 flag = iter.key().ulongValue;
        if ((value & flag) == flag)
        {
            //flag is set
            arr.append(qMakePair(iter.value(), flag));
        }
    }

    //now we have all flags, check if some overlap
    for (int i = 0; i < arr.size(); ++i)
    {
        const quint64 firstFlag = arr.at(i).second;
        for (int j = 0; j < arr.size();)
        {
            if (j == i)
            {
                j++;
                continue;
            }
            //check if they overlap
            quint64 secondFlag = arr.at(j).second;
            if ((firstFlag & secondFlag) == secondFlag)
            {
                //they overlap, remove the second flag
                removeFromArray(arr, j);
                if (j < i)
                    i--; // i was pushed back by one as well
            }
            else
                j++;
        }
    }

    //if array has zero elements just return the value in hexadecimal
    if (arr.size() == 0)
    {
        return i18n("0x%1 (no matching flags)", QString::number(value, 16));
    }


    //make sure all we also show remaining bits at the end
    quint64 usedBits = 0;
    QString result;
    for (int i = 0, size = arr.size(); i < size; ++i)
    {
        if (i != 0)
            result += QLatin1String(" | ");
        result += arr.at(i).first;
        usedBits |= arr.at(i).second;
    }
    //TODO remove a NONE flag if others present (value = 0)

    //TODO set invalid if value not completely covered by flags

    if (usedBits != value)
    {
        quint64 missing = value & ~usedBits;
        result += QLatin1String(" | 0x") + QString::number(missing, 16);
    }

    return result;
}

QString FlagDataInformation::typeName() const
{
    return QLatin1String("flags");
}

QString FlagDataInformation::getTypeString() const
{
    return i18nc("bit flags, i.e. 'R | W | X' with undelying type", "flags(%1)", mValue->typeName());
}
