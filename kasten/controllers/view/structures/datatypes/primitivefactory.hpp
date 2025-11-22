/*
    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVEFACTORY_HPP
#define KASTEN_PRIMITIVEFACTORY_HPP

// lib
#include "primitivedatainformation.hpp"
// Std
#include <memory>

struct LoggerWithContext;
class QString;

namespace PrimitiveFactory {

/** Converts @p typeStr to a PrimitiveDataType case-insensitively
 * @return The corresponding type or PrimitiveDataType::Invalid if string is not regcognized
 */
[[nodiscard]]
PrimitiveDataType typeStringToType(const QString& string, const LoggerWithContext& logger);

[[nodiscard]]
std::unique_ptr<PrimitiveDataInformation> newInstance(const QString& name, PrimitiveDataType type,
                                                      const LoggerWithContext& logger, DataInformation* parent = nullptr);

[[nodiscard]]
inline std::unique_ptr<PrimitiveDataInformation> newInstance(const QString& name, const QString& typeString,
                                                             const LoggerWithContext& logger, DataInformation* parent = nullptr)
{
    return newInstance(name, typeStringToType(typeString, logger), logger, parent);
}

}

#endif /* KASTEN_PRIMITIVEFACTORY_HPP */
