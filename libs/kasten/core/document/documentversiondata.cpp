/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentversiondata.hpp"

namespace Kasten {

DocumentVersionData::DocumentVersionData(DocumentVersionId id, const QString& changeComment)
    : mId(id)
    , mChangeComment(changeComment)
{}

DocumentVersionId DocumentVersionData::id() const { return mId; }

QString DocumentVersionData::changeComment() const { return mChangeComment; }

}
