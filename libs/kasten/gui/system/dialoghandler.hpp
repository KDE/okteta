/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DIALOGHANDLER_HPP
#define KASTEN_DIALOGHANDLER_HPP

// lib
#include "kastengui_export.hpp"
// Kasten core
#include <Kasten/AbstractOverwriteDialog>
#include <Kasten/AbstractSaveDiscardDialog>

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
