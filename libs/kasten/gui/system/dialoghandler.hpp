/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef DIALOGHANDLER_HPP
#define DIALOGHANDLER_HPP

// lib
#include <kasten/kastengui_export.hpp>
// Kasten core
#include <kasten/abstractoverwritedialog.hpp>
#include <kasten/abstractsavediscarddialog.hpp>

class QWidget;

namespace Kasten {

class KASTENGUI_EXPORT DialogHandler : public AbstractOverwriteDialog
                                     , public AbstractSaveDiscardDialog
{
public:
    explicit DialogHandler(QWidget* widget = nullptr);

    ~DialogHandler() override;

public: // AbstractOverwriteDialog API
    Answer queryOverwrite(const QUrl& url, const QString& title) const override;

public: // AbstractSaveDiscardDialog API
    Answer queryDiscardOnReload(const AbstractDocument* document, const QString& title) const override;
    Answer querySaveDiscard(const AbstractDocument* document, const QString& title) const override;
    Answer queryDiscard(const AbstractDocument* document, const QString& title) const override;

public:
    void setWidget(QWidget* widget);

private:
    QWidget* mWidget;
};

}

#endif
