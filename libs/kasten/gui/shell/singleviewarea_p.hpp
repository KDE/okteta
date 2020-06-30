/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINGLEVIEWAREA_P_HPP
#define KASTEN_SINGLEVIEWAREA_P_HPP

// lib
#include "abstractviewarea_p.hpp"
#include "singleviewarea.hpp"
#include "viewareabox.hpp"
#include "abstractview.hpp"

class QDragMoveEvent;
class QDropEvent;

namespace Kasten {

class SingleViewAreaPrivate : public AbstractViewAreaPrivate
{
public:
    explicit SingleViewAreaPrivate(SingleViewArea* parent);

    ~SingleViewAreaPrivate() override;

public:
    void init();

public: // AbstractViewArea API
    void setFocus();
    QWidget* widget() const;
    bool hasFocus() const;

public: // If::ToolInlineViewable API
    void setCurrentToolInlineView(AbstractToolInlineView* view);
    AbstractToolInlineView* currentToolInlineView() const;

public:
    void setView(AbstractView* view);

private:
    Q_DECLARE_PUBLIC(SingleViewArea)

private:
    ViewAreaBox* mViewAreaBox = nullptr;
    AbstractView* mCurrentView = nullptr;
    AbstractToolInlineView* mCurrentToolInlineView = nullptr;
};

inline QWidget* SingleViewAreaPrivate::widget()  const { return mViewAreaBox; }
inline bool SingleViewAreaPrivate::hasFocus()    const
{
    return mCurrentView ? mCurrentView->hasFocus() : false;
}

inline void SingleViewAreaPrivate::setFocus()    { mCurrentView->setFocus(); }

inline AbstractToolInlineView* SingleViewAreaPrivate::currentToolInlineView() const
{
    return mCurrentToolInlineView;
}

}

#endif
