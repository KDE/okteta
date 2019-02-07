/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_ABSTRACTLINEPOPUP_HPP
#define KASTEN_ABSTRACTLINEPOPUP_HPP

// lib
#include <kasten/kastengui_export.hpp>
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
    explicit AbstractLinePopup(QWidget* parent = nullptr);
    ~AbstractLinePopup() override;

public:
    void setIcon(const QIcon& icon);
    void setPosition(const QPoint& globalPosition);
    void setWidget(QWidget* widget);

    int exec();

protected: // QObject API
    bool eventFilter(QObject* object, QEvent* event) override;

protected: // QWidget API
    void setVisible(bool visible) override;

protected:
    void setResult(int result);

private:
    const QScopedPointer<class AbstractLinePopupPrivate> d;
};

}

#endif
