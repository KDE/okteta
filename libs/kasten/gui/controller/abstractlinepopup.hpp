/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTLINEPOPUP_HPP
#define KASTEN_ABSTRACTLINEPOPUP_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QWidget>

class QIcon;

namespace Kasten {

// TODO: find better name then popup for this kind of view
// TODO: add cancel/ok button concept of KDialog, add to the right end
// TODO: add option to resize, perhaps also move
// TODO: add option to add a pointer to the border, to show origin

// a minimal dialog
class KASTENGUI_EXPORT AbstractLinePopup : public QWidget
{
    Q_OBJECT

public:
    enum DialogCode
    {
        Rejected = 0,
        Accepted = 1,
    };

public:
    explicit AbstractLinePopup(QWidget* parent = nullptr);
    ~AbstractLinePopup() override;

public:
    void setIcon(const QIcon& icon);
    void setPosition(const QPoint& globalPosition);
    void setWidget(QWidget* widget);

    int exec();
    void open();
    int result() const;

Q_SIGNALS:
    void finished(int result);

public Q_SLOTS:
    void accept(); // TODO: virtual
    void reject(); // TODO: virtual

protected: // QObject API
    bool eventFilter(QObject* object, QEvent* event) override;

protected: // QWidget API
    void setVisible(bool visible) override;

protected:
    void setResult(int result);

private:
    const QScopedPointer<class AbstractLinePopupPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AbstractLinePopup)
};

}

#endif
