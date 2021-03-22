/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ZOOMPINCHCONTROLLER_HPP
#define OKTETA_ZOOMPINCHCONTROLLER_HPP

class QPinchGesture;

namespace Okteta {
class AbstractByteArrayView;

class ZoomPinchController
{
public:
    explicit ZoomPinchController(AbstractByteArrayView* view);

public:
    bool handlePinchGesture(QPinchGesture* pinchGesture);

private:
    AbstractByteArrayView* mView;
    double mOriginalZoomLevel = -1;
};

}

#endif
