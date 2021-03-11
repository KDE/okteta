/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELDATAGENERATOR_HPP
#define KASTEN_ABSTRACTMODELDATAGENERATOR_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>
#include <QString>

class QMimeData;

namespace Kasten {

class AbstractModelDataGeneratorPrivate;

class KASTENCORE_EXPORT AbstractModelDataGenerator : public QObject
{
    Q_OBJECT

public:
    enum Flags
    {
        StaticGeneration,
        DynamicGeneration
    };

protected:
    AbstractModelDataGenerator(AbstractModelDataGeneratorPrivate* d);

public:
    AbstractModelDataGenerator(const QString& typeName, const QString& mimeType, Flags flags);

    ~AbstractModelDataGenerator() override;

public: // API to be implemented
    virtual QMimeData* generateData() = 0;

public:
    QString typeName() const;
    QString mimeType() const;
    Flags flags() const;

protected:
    const QScopedPointer<AbstractModelDataGeneratorPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractModelDataGenerator)
};

}

#endif
