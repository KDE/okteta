/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTVIEW_HPP
#define KASTEN_ABSTRACTVIEW_HPP

// lib
#include "kastengui_export.hpp"
// Kasten core
#include <Kasten/AbstractDocument>

class QWidget;

namespace Kasten {

// TODO: is there a common base for view and document?
class KASTENGUI_EXPORT AbstractView : public AbstractModel
{
    Q_OBJECT

public:
    explicit AbstractView(AbstractModel* baseModel = nullptr);
    ~AbstractView() override;

public: // API to be implemented
    virtual void setFocus() = 0;

    virtual QWidget* widget() const = 0;
    virtual bool hasFocus() const = 0;

Q_SIGNALS:
    // view has focus in the window
    void focusChanged(bool hasFocus);
};

}

#endif
