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
// Std
#include <memory>

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
    KASTENCORE_NO_EXPORT explicit AbstractModelDataGenerator(std::unique_ptr<AbstractModelDataGeneratorPrivate>&& dd);

public:
    AbstractModelDataGenerator(const QString& typeName, const QString& mimeType, Flags flags);

    ~AbstractModelDataGenerator() override;

public: // API to be implemented
    [[nodiscard]]
    virtual std::unique_ptr<QMimeData> generateData() = 0;

public:
    [[nodiscard]]
    QString typeName() const;
    [[nodiscard]]
    QString mimeType() const;
    [[nodiscard]]
    Flags flags() const;

protected:
    const std::unique_ptr<AbstractModelDataGeneratorPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractModelDataGenerator)
};

}

#endif
