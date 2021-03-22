/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_TOUCHONLYTAPANDHOLDGESTURERECOGNIZER_HPP
#define OKTETA_TOUCHONLYTAPANDHOLDGESTURERECOGNIZER_HPP

// Qt
#include <QGestureRecognizer>

namespace Okteta {

class TouchOnlyTapAndHoldGestureRecognizer : public QGestureRecognizer
{
public:
    TouchOnlyTapAndHoldGestureRecognizer();

public:
    QGesture* create(QObject* target) override;
    QGestureRecognizer::Result recognize(QGesture* state, QObject* watched, QEvent* event) override;
    void reset(QGesture* state) override;
};

}

#endif
