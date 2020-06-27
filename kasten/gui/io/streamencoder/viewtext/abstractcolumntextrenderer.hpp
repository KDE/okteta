/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTCOLUMNTEXTRENDERER_HPP
#define KASTEN_ABSTRACTCOLUMNTEXTRENDERER_HPP

class QTextStream;
class QString;

namespace Kasten {

/**
 * interface for the text export of columns
 * @author Friedrich W. H. Kossebau <kossebau@kde.org>
 */
class AbstractColumnTextRenderer
{
protected:
    static QString whiteSpace(unsigned int length);

public:
    virtual ~AbstractColumnTextRenderer();

public: // API to be implemented
    virtual void renderFirstLine(QTextStream* stream, int lineIndex) const = 0;
    virtual void renderNextLine(QTextStream* stream, bool isSubline = false) const = 0;
    /// default returns 1
    virtual int noOfSublinesNeeded() const;
};

}

#endif
