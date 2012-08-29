/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef PRIMITIVEFACTORY_H_
#define PRIMITIVEFACTORY_H_

#include "primitivedatatype.h"

class PrimitiveDataInformation;
struct LoggerWithContext;
class QString;
class DataInformation;

namespace PrimitiveFactory
{

/** Converts @p typeStr to a PrimitiveDataType case-insensitively
 * @return The corresponding type or PrimitiveDataTypeEnum::Type_Invalid if string is not regcognized
 */
PrimitiveDataType typeStringToType(const QString& typeStr, const LoggerWithContext& logger);

PrimitiveDataInformation* newInstance(const QString& name, PrimitiveDataType type,
        const LoggerWithContext& logger, DataInformation* parent = 0);

inline PrimitiveDataInformation* newInstance(const QString& name, const QString& typeString,
        const LoggerWithContext& logger, DataInformation* parent = 0)
{
    return newInstance(name, typeStringToType(typeString, logger), logger, parent);
}

}

#endif /* PRIMITIVEFACTORY_H_ */
