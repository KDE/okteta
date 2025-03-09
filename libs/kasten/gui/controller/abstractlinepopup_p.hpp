/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTLINEPOPUP_P_HPP
#define KASTEN_ABSTRACTLINEPOPUP_P_HPP

#include "abstractlinepopup.hpp"

class QIcon;
class QLabel;
class QHBoxLayout;
class QEventLoop;

namespace Kasten {

class AbstractLinePopupPrivate
{
public:
    explicit AbstractLinePopupPrivate(AbstractLinePopup* parent);
    AbstractLinePopupPrivate(const AbstractLinePopupPrivate&) = delete;
    AbstractLinePopupPrivate(AbstractLinePopupPrivate&&) = delete;

    ~AbstractLinePopupPrivate() = default; // trivial

    AbstractLinePopupPrivate& operator=(const AbstractLinePopupPrivate&) = delete;
    AbstractLinePopupPrivate& operator=(AbstractLinePopupPrivate&&) = delete;

public:
    void init();

    void setPosition(QPoint globalPosition);
    void setIcon(const QIcon& icon);
    void setWidget(QWidget* widget);

    void setResult(int result);
    [[nodiscard]]
    int exec();
    void open();
    void accept();
    void reject();

    [[nodiscard]]
    int result() const;

public: // QWidget API
    void setVisible(bool visible);

public:
    [[nodiscard]]
    bool isOrContainsObject(QObject* object) const;

private:
    void done(int result);

private:
    AbstractLinePopup* const p;

    QHBoxLayout* mBaseLayout;
    QLabel* mIconLabel;
    QWidget* mWidget = nullptr;

    QEventLoop* mEventLoop = nullptr;
    int mResult = AbstractLinePopup::Rejected;
};

inline AbstractLinePopupPrivate::AbstractLinePopupPrivate(AbstractLinePopup* parent)
    : p(parent)
{
}

inline int AbstractLinePopupPrivate::result() const { return mResult; }
inline void AbstractLinePopupPrivate::setResult(int result) { mResult = result; }

inline bool AbstractLinePopupPrivate::isOrContainsObject(QObject* object) const
{
    bool result = false;

    while (object) {
        if (object == p) {
            result = true;
            break;
        }
        object = object->parent();
    }

    return result;
}

}

#endif
