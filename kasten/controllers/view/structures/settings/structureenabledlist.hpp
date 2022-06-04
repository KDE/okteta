/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTUREENABLEDLIST_HPP
#define STRUCTUREENABLEDLIST_HPP

// Qt
#include <QVector>
#include <QHash>

class QStringList;

class StructureEnabledData
{
public:
    explicit StructureEnabledData(const QString& id, const QString& structure);
    StructureEnabledData(const StructureEnabledData& other) = default;

public:
    QString id;
    QString structure;
    bool isEnabled;
};

inline StructureEnabledData::StructureEnabledData(const QString& id, const QString& structure)
    : id(id)
    , structure(structure)
    , isEnabled(true)
{
}

Q_DECLARE_TYPEINFO(StructureEnabledData, Q_MOVABLE_TYPE);


// Any actively disabled entries are keot in place,
// in case it gets enabled again during editing
// so does not lose any previous position and structures.
class StructureEnabledList
{
public:
    StructureEnabledList() = default;

public:
    void setEnabledStructures(const QStringList& enabledStructures);
    // drop no longer existing structures from the enabled list
    void removeStructures(const QHash<QString, QStringList>& structures);

    void setEnabled(const QString& id, bool  isEnabled);

    bool isEnabled(const QString& id) const;
    QStringList toQStringList() const;

    QVector<StructureEnabledData>::ConstIterator begin() const;
    QVector<StructureEnabledData>::ConstIterator end() const;

private:
    QVector<StructureEnabledData> m_enabledList;
};

inline QVector<StructureEnabledData>::ConstIterator StructureEnabledList::begin() const
{
    return m_enabledList.begin();
}

inline QVector<StructureEnabledData>::ConstIterator StructureEnabledList::end() const
{
    return m_enabledList.end();
}

#endif
