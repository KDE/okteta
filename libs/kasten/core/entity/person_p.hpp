/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PERSON_P_HPP
#define KASTEN_PERSON_P_HPP

#include "person.hpp"

// Qt
#include <QString>
#include <QIcon>

namespace Kasten {

class PersonPrivate : public QSharedData
{
public:
    PersonPrivate(const QString& name, const QIcon& faceIcon);
    PersonPrivate() = delete;

    ~PersonPrivate();

public:
    QString name() const;
    QIcon faceIcon() const;

private:
    QString mName;
    QIcon mFaceIcon;
};

inline PersonPrivate::PersonPrivate(const QString& name, const QIcon& faceIcon)
    : mName(name)
    , mFaceIcon(faceIcon)
{}

inline PersonPrivate::~PersonPrivate() = default;

inline QString PersonPrivate::name()   const { return mName; }
inline QIcon PersonPrivate::faceIcon() const { return mFaceIcon; }

}

#endif
