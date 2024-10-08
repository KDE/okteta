/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTEXPORTJOB_HPP
#define KASTEN_ABSTRACTEXPORTJOB_HPP

// lib
#include "kastencore_export.hpp"
// KF
#include <KJob>
// Std
#include <memory>

namespace Kasten {

class AbstractExportJobPrivate;

class KASTENCORE_EXPORT AbstractExportJob : public KJob
{
    Q_OBJECT

protected:
    KASTENCORE_NO_EXPORT explicit AbstractExportJob(std::unique_ptr<AbstractExportJobPrivate>&& dd);

public:
    AbstractExportJob();

    ~AbstractExportJob() override;

protected:
    const std::unique_ptr<AbstractExportJobPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractExportJob)
};

}

#endif
