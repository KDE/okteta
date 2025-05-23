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
    TouchOnlyTapAndHoldGestureRecognizer(const TouchOnlyTapAndHoldGestureRecognizer&) = delete;
    TouchOnlyTapAndHoldGestureRecognizer(TouchOnlyTapAndHoldGestureRecognizer&&) = delete;

    TouchOnlyTapAndHoldGestureRecognizer& operator=(const TouchOnlyTapAndHoldGestureRecognizer&) = delete;
    TouchOnlyTapAndHoldGestureRecognizer& operator=(TouchOnlyTapAndHoldGestureRecognizer&&) = delete;

public:
    [[nodiscard]]
    QGesture* create(QObject* target) override;
    [[nodiscard]]
    QGestureRecognizer::Result recognize(QGesture* state, QObject* object, QEvent* event) override;
    void reset(QGesture* state) override;
};

}

#endif
