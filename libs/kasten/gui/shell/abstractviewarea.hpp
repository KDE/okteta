/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTVIEWAREA_HPP
#define KASTEN_ABSTRACTVIEWAREA_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QObject>
// Std
#include <memory>

class QWidget;

namespace Kasten {

class AbstractViewAreaPrivate;

// TODO: rename abstractdocumentviewarea?
class KASTENGUI_EXPORT AbstractViewArea : public QObject
{
    Q_OBJECT

protected:
    AbstractViewArea();
    KASTENGUI_NO_EXPORT explicit AbstractViewArea(std::unique_ptr<AbstractViewAreaPrivate>&& dd);

public:
    ~AbstractViewArea() override;

public: // API to be implemented
    virtual void setFocus() = 0;

    [[nodiscard]]
    virtual QWidget* widget() const = 0;
//     virtual QString title() const = 0;
//     virtual AbstractTool* tool() const = 0;
    [[nodiscard]]
    virtual bool hasFocus() const = 0;

Q_SIGNALS:
    // area has focus in the window
    void focusChanged(bool hasFocus);

protected:
    const std::unique_ptr<AbstractViewAreaPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractViewArea)
};

}

#endif
