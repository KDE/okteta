/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETABROWSEREXTENSION_HPP
#define OKTETABROWSEREXTENSION_HPP

// KF
#include <KParts/BrowserExtension>

class OktetaPart;

/**
 * @short Extension for better support for embedding in browsers
 * @author Friedrich W. H. Kossebau <kossebau@kde.org>
 */
class OktetaBrowserExtension : public KParts::BrowserExtension
{
    Q_OBJECT

public:
    explicit OktetaBrowserExtension(OktetaPart* part);

public: // KParts::BrowserExtension API
    void saveState(QDataStream& stream) override;
    void restoreState(QDataStream& stream) override;

public Q_SLOTS:
    /** copy text to clipboard */
    void copy();
    void print();

private Q_SLOTS:
    /** selection has changed */
    void onSelectionChanged(bool hasSelection);

private:
    OktetaPart* mPart;
};

#endif
