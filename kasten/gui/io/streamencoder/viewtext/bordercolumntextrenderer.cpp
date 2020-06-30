/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bordercolumntextrenderer.hpp"

// Qt
#include <QTextStream>

namespace Kasten {

void BorderColumnTextRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    Q_UNUSED(lineIndex)
    render(stream);
}

void BorderColumnTextRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    Q_UNUSED(isSubline)
    render(stream);
}

void BorderColumnTextRenderer::render(QTextStream* stream) const
{
    *stream << " | ";
}

}
