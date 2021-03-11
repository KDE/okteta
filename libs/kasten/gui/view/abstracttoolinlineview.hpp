/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTTOOLINLINEVIEW_HPP
#define KASTEN_ABSTRACTTOOLINLINEVIEW_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QObject>

class QWidget;
class QString;

namespace Kasten {

class AbstractTool;

// TODO: is there a common base for view and document?
class KASTENGUI_EXPORT AbstractToolInlineView : public QObject
{
    Q_OBJECT

protected:
    AbstractToolInlineView();

public:
    ~AbstractToolInlineView() override;

public: // API to be implemented
    virtual QWidget* widget() const = 0;
    virtual QString title() const = 0;
    virtual AbstractTool* tool() const = 0;

public:
    void activate();

private:
    const QScopedPointer<class AbstractToolInlineViewPrivate> d_ptr;
};

}

#endif
