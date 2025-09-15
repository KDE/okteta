/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FONTSCALINGZOOMSTATETEST_HPP
#define FONTSCALINGZOOMSTATETEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class FontScalingZoomStateTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testNormalZoomScale();
    void testZoomLevels();
    void testSetFontSize();
};

}

#endif
