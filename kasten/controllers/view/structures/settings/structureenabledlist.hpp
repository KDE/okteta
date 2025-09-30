/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTUREENABLEDLIST_HPP
#define STRUCTUREENABLEDLIST_HPP

// Qt
#include <QString>
// Std
#include <unordered_map>
#include <vector>

class QStringList;

class StructureEnabledData
{
public:
    explicit StructureEnabledData(const QString& id, const QString& structure);
    StructureEnabledData(StructureEnabledData&& other) = default;
    StructureEnabledData(const StructureEnabledData& other) = delete;

    StructureEnabledData& operator=(StructureEnabledData&& other) = default;
    StructureEnabledData& operator=(const StructureEnabledData& other) = delete;

public:
    QString id;
    QString structure;
    bool isEnabled = true;
};

inline StructureEnabledData::StructureEnabledData(const QString& id, const QString& structure)
    : id(id)
    , structure(structure)
{
}

// Any actively disabled entries are keot in place,
// in case it gets enabled again during editing
// so does not lose any previous position and structures.
class StructureEnabledList
{
public:
    StructureEnabledList() = default;
    StructureEnabledList(const StructureEnabledList& other) = delete;
    StructureEnabledList(StructureEnabledList&& other) = delete;

public:
    StructureEnabledList& operator=(const StructureEnabledList& other) = delete;
    StructureEnabledList& operator=(StructureEnabledList&& other) = delete;

public:
    void setEnabledStructures(const QStringList& enabledStructures);
    // drop no longer existing structures from the enabled list
    void removeStructures(const std::unordered_map<QString, QStringList>& structures);

    void setEnabled(const QString& id, bool  isEnabled);

    [[nodiscard]]
    bool isEnabled(const QString& id) const;
    [[nodiscard]]
    QStringList toQStringList() const;

    [[nodiscard]]
    std::vector<StructureEnabledData>::const_iterator begin() const;
    [[nodiscard]]
    std::vector<StructureEnabledData>::const_iterator end() const;

private:
    std::vector<StructureEnabledData> m_enabledList;
};

inline std::vector<StructureEnabledData>::const_iterator StructureEnabledList::begin() const
{
    return m_enabledList.begin();
}

inline std::vector<StructureEnabledData>::const_iterator StructureEnabledList::end() const
{
    return m_enabledList.end();
}

#endif
