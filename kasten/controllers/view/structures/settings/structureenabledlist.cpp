/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureenabledlist.hpp"

// Qt
#include <QRegularExpression>
#include <QStringList>

namespace {
inline QString wildCard() { return QStringLiteral("*"); }
}

void StructureEnabledList::setEnabledStructures(const QStringList& enabledStructures)
{
    m_enabledList.clear();

    QRegularExpression regex(QStringLiteral("'(.+)':'(.+)'"));
    m_enabledList.reserve(enabledStructures.size());
    for (const QString& expression : enabledStructures) {
        QRegularExpressionMatch match = regex.match(expression);
        if (match.hasMatch()) {
            const QString id = match.captured(1);
            const QString structure = match.captured(2);
            m_enabledList.append(StructureEnabledData(id, structure));
        }
    }
}

void StructureEnabledList::removeStructures(const QHash<QString, QStringList>& structures)
{
    auto it = m_enabledList.begin();
    while (it != m_enabledList.end()) {
        auto idIt = structures.constFind(it->id);
        bool remove = false;
        if (idIt != structures.constEnd()) {
            if ((it->structure != ::wildCard()) && !idIt.value().contains(it->structure)) {
                remove = true;
            }
        } else {
            remove = true;
        }
        if (remove) {
            it = m_enabledList.erase(it);
        } else {
            ++it;
        }
    }
}

void StructureEnabledList::setEnabled(const QString& id, bool  isEnabled)
{
    bool isListed = false;
    for (StructureEnabledData& data : m_enabledList) {
        if (data.id == id) {
            isListed = true;
            data.isEnabled = isEnabled;
        }
    }
    if (!isListed && isEnabled) {
        m_enabledList.append(StructureEnabledData(id, {wildCard()}));
    }
}

bool StructureEnabledList::isEnabled(const QString& id) const
{
    return std::any_of(m_enabledList.begin(), m_enabledList.end(), [&id](const StructureEnabledData& data) {
        return (data.id == id) && data.isEnabled;
    });
}

QStringList StructureEnabledList::toQStringList() const
{
    const QString expressionTemplate = QStringLiteral("\'%1\':\'%2\'");

    QStringList enabledStructures;
    for (const StructureEnabledData& data : m_enabledList) {
        if (data.isEnabled) {
            enabledStructures.append(expressionTemplate.arg(data.id, data.structure));
        }
    }
    return enabledStructures;
}
