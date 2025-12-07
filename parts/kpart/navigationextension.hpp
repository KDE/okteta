/*
    SPDX-FileCopyrightText: 2004, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETANAVIGATIONEXTENSION_HPP
#define OKTETANAVIGATIONEXTENSION_HPP

// KF
#include <KParts/NavigationExtension>

class OktetaPart;

/**
 * @short Extension for better support for embedding in browsers
 * @author Friedrich W. H. Kossebau <kossebau@kde.org>
 */
class OktetaNavigationExtension : public KParts::NavigationExtension
{
    Q_OBJECT

public:
    explicit OktetaNavigationExtension(OktetaPart* part);

public: // KParts::NavigationExtension API
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
    OktetaPart* const mPart;
};

#endif
