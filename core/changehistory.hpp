/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHANGEHISTORY_HPP
#define OKTETA_CHANGEHISTORY_HPP

// lib
#include "bytearraychange.hpp"
// Qt
#include <QVector>
#include <QtPlugin>

class QByteArray;

namespace Okteta {

// TODO: split readonly part into own interface
class ChangeHistory
{
public:
    virtual ~ChangeHistory();

public: // get
    virtual QVector<ByteArrayChange> changes(int firstVersionIndex, int lastVersionIndex) const = 0;
    virtual QByteArray initialData() const = 0;

public: // set
    virtual void doChanges(const QVector<Okteta::ByteArrayChange>& changes,
                           int oldVersionIndex, int newVersionIndex) = 0;

public: // signal
    virtual void changesDone(const QVector<Okteta::ByteArrayChange>& changes,
                             int oldVersionIndex, int newVersionIndex) = 0;
};

inline ChangeHistory::~ChangeHistory() = default;

}

Q_DECLARE_INTERFACE(Okteta::ChangeHistory, "org.kde.okteta.changehistory/1.0")

#endif
