/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTVERSIONDATA_HPP
#define KASTEN_DOCUMENTVERSIONDATA_HPP

// lib
#include "kastencore_export.hpp"
#include "documentversionid.hpp"
// Qt
#include <QString>

namespace Kasten {

class KASTENCORE_EXPORT DocumentVersionData
{
public:
    DocumentVersionData(DocumentVersionId id, const QString& changeComment);
    DocumentVersionData() = delete;

public:
    DocumentVersionId id() const;
    QString changeComment() const;

private:
    DocumentVersionId mId;
    QString mChangeComment;
};

}

#endif
