/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELEXPORTER_HPP
#define KASTEN_ABSTRACTMODELEXPORTER_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QObject>
#include <QString>
// Std
#include <memory>

class QUrl;

namespace Kasten {

class AbstractModel;
class AbstractModelSelection;
class AbstractExportJob;

class AbstractModelExporterPrivate;

class KASTENCORE_EXPORT AbstractModelExporter : public QObject
{
    Q_OBJECT

protected:
    KASTENCORE_NO_EXPORT explicit AbstractModelExporter(std::unique_ptr<AbstractModelExporterPrivate>&& dd);

public:
    AbstractModelExporter(const QString& remoteTypeName, const QString& remoteMimeType);

    ~AbstractModelExporter() override;

public: // API to be implemented
    virtual AbstractExportJob* startExport(AbstractModel* model, const AbstractModelSelection* selection,
                                           const QUrl& url) = 0;
    [[nodiscard]]
    virtual QString modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const = 0;

public:
    [[nodiscard]]
    QString remoteTypeName() const;
    [[nodiscard]]
    QString remoteMimeType() const;

protected:
    const std::unique_ptr<AbstractModelExporterPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractModelExporter)
};

}

#endif
