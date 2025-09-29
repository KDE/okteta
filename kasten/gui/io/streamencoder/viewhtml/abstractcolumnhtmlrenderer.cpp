/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcolumnhtmlrenderer.hpp"

// Qt
#include <QString>

namespace Kasten {

AbstractColumnHtmlRenderer::~AbstractColumnHtmlRenderer() = default;

QString AbstractColumnHtmlRenderer::whiteSpace(unsigned int length)
{
    return QStringLiteral("&nbsp;").repeated(length);
}

QString AbstractColumnHtmlRenderer::emptyCells(unsigned int length, unsigned int codingWidth)
{
    if (length == 0) {
        return {};
    }
    if (codingWidth == 0) {
        return QStringLiteral("<td colspan=%1></td>").arg(length);
    }
    return QString(QLatin1String("<td><tt>") + whiteSpace(codingWidth) + QLatin1String("</tt></td>")).repeated(length);
    // TODO: use colspan where possible and spaces only for empty columns
}

QString AbstractColumnHtmlRenderer::htmlEscaped(QChar c)
{
    if (c.isSpace()) {
        return QStringLiteral("&nbsp;");
    }
    return QString(c).toHtmlEscaped();
}

int AbstractColumnHtmlRenderer::noOfSublinesNeeded() const { return DefaultNoOfSublines; }

}
