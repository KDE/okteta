/*
    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ZOOMPINCHCONTROLLER_HPP
#define OKTETA_ZOOMPINCHCONTROLLER_HPP

class QPinchGesture;

namespace Okteta {
class AbstractByteArrayViewPrivate;

class ZoomPinchController
{
public:
    explicit ZoomPinchController(AbstractByteArrayViewPrivate* view);
    ZoomPinchController(const ZoomPinchController&) = delete;
    ZoomPinchController(ZoomPinchController&&) = delete;

    ~ZoomPinchController() = default;

    ZoomPinchController& operator=(const ZoomPinchController&) = delete;
    ZoomPinchController& operator=(ZoomPinchController&&) = delete;

public:
    [[nodiscard]]
    bool handlePinchGesture(QPinchGesture* pinchGesture);

private:
    AbstractByteArrayViewPrivate* const mView;

    double m_originalZoomScale = -1;
};

}

#endif
