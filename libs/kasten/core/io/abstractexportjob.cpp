/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractexportjob.hpp"
#include "abstractexportjob_p.hpp"

namespace Kasten {

AbstractExportJob::AbstractExportJob(std::unique_ptr<AbstractExportJobPrivate>&& dd)
    : d_ptr(std::move(dd))
{}

AbstractExportJob::AbstractExportJob()
    : d_ptr(std::make_unique<AbstractExportJobPrivate>(this))
{}

AbstractExportJob::~AbstractExportJob() = default;

}

#include "moc_abstractexportjob.cpp"
