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
class AbstractByteArrayViewPrivate;

class Dropper
{
public:
    explicit Dropper(AbstractByteArrayViewPrivate* view);
    Dropper(const Dropper&) = delete;
    Dropper(Dropper&&) = delete;

    ~Dropper() = default; // trivial

    Dropper& operator=(const Dropper&) = delete;
    Dropper& operator=(Dropper&&) = delete;

public: // AbstractMouseController API
    [[nodiscard]]
    bool handleDragEnterEvent(QDragEnterEvent* dragEnterEvent);
    [[nodiscard]]
    bool handleDragMoveEvent(QDragMoveEvent* dragMoveEvent);
    [[nodiscard]]
    bool handleDragLeaveEvent(QDragLeaveEvent* dragLeaveEvent);
    [[nodiscard]]
    bool handleDropEvent(QDropEvent* dropEvent);

public:
    [[nodiscard]]
    bool isActive() const;

private:
    void handleInternalDrag(QDropEvent* dropEvent, AbstractByteArrayViewPrivate* sourceByteArrayView);

private:
    AbstractByteArrayViewPrivate* const mByteArrayView;

    Address mBeforeDragCursorPos;
    bool mBeforeDragCursorIsBehind : 1;
    bool mCursorIsMovedByDrag : 1;

    bool mIsActive : 1;
};

inline Dropper::Dropper(AbstractByteArrayViewPrivate* view)
    : mByteArrayView(view)
    , mIsActive(false)
{
}

inline bool Dropper::isActive() const
{
    return mIsActive;
}

}

#endif
