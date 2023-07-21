/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTCOLUMNHTMLRENDERER_HPP
#define KASTEN_ABSTRACTCOLUMNHTMLRENDERER_HPP

class QTextStream;
class QString;
class QChar;

namespace Kasten {

/**
 * interface for the HTML export of columns
 */
class AbstractColumnHtmlRenderer
{
private:
    static inline constexpr int DefaultNoOfSublines = 1;

protected:
    static QString whiteSpace(unsigned int length);
    static QString emptyCells(unsigned int length, unsigned int codingWidth);
    static QString htmlEscaped(QChar c);

protected:
    AbstractColumnHtmlRenderer() = default;

public:
    AbstractColumnHtmlRenderer(const AbstractColumnHtmlRenderer&) = delete;
    virtual ~AbstractColumnHtmlRenderer();

    AbstractColumnHtmlRenderer& operator=(const AbstractColumnHtmlRenderer&) = delete;

public: // API to be implemented
    virtual void renderFirstLine(QTextStream* stream, int lineIndex) const = 0;
    virtual void renderNextLine(QTextStream* stream, bool isSubline = false) const = 0;
    /// default returns 1
    virtual int noOfSublinesNeeded() const;
};

}

#endif
