/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_DROPPER_HPP
#define OKTETA_DROPPER_HPP

// Okteta core
#include <Okteta/Address>

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

namespace Okteta {
class AbstractByteArrayView;

class Dropper
{
public:
    explicit Dropper(AbstractByteArrayView* view);
    Dropper() = delete;

    ~Dropper();

public: // AbstractMouseController API
    bool handleDragEnterEvent(QDragEnterEvent* dragEnterEvent);
    bool handleDragMoveEvent(QDragMoveEvent* dragMoveEvent);
    bool handleDragLeaveEvent(QDragLeaveEvent* dragLeaveEvent);
    bool handleDropEvent(QDropEvent* dropEvent);

public:
    bool isActive() const;

private:
    void handleInternalDrag(QDropEvent* dropEvent, AbstractByteArrayView* sourceByteArrayView);

private:
    AbstractByteArrayView* mByteArrayView;

    Address mBeforeDragCursorPos;
    bool mBeforeDragCursorIsBehind : 1;
    bool mCursorIsMovedByDrag : 1;

    bool mIsActive : 1;
};

}

#endif
