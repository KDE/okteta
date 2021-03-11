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

class QWidget;

namespace Kasten {

class AbstractViewAreaPrivate;

// TODO: rename abstractdocumentviewarea?
class KASTENGUI_EXPORT AbstractViewArea : public QObject
{
    Q_OBJECT

protected:
    AbstractViewArea();
    explicit AbstractViewArea(AbstractViewAreaPrivate* d);

public:
    ~AbstractViewArea() override;

public: // API to be implemented
    virtual void setFocus() = 0;

    virtual QWidget* widget() const = 0;
//     virtual QString title() const = 0;
//     virtual AbstractTool* tool() const = 0;
    virtual bool hasFocus() const = 0;

Q_SIGNALS:
    // area has focus in the window
    void focusChanged(bool hasFocus);

protected:
    const QScopedPointer<AbstractViewAreaPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractViewArea)
};

}

#endif
