/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_DATATYPEDISPLAYSTRINGIFIERMANAGER_HPP
#define OKTETA_DATATYPEDISPLAYSTRINGIFIERMANAGER_HPP

// Std
#include <memory>
#include <unordered_map>

namespace Okteta {

class AbstractDataTypeDisplayStringifier;

class DataTypeDisplayStringifierManager
{
public:
    DataTypeDisplayStringifierManager();
    ~DataTypeDisplayStringifierManager();

    DataTypeDisplayStringifierManager(const DataTypeDisplayStringifierManager&) = delete;

    DataTypeDisplayStringifierManager& operator=(const DataTypeDisplayStringifierManager&) = delete;

public:
    [[nodiscard]]
    AbstractDataTypeDisplayStringifier* stringifier(int metaTypeId) const;

private:
    std::unordered_map<int, std::unique_ptr<AbstractDataTypeDisplayStringifier>> m_stringifiers;
};

}

#endif
