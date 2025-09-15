/*
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

namespace Kasten {

class AbstractUserMessagesHandler;

class KASTENGUI_EXPORT DialogHandler : public AbstractOverwriteDialog
                                     , public AbstractSaveDiscardDialog
{
public:
    explicit DialogHandler(AbstractUserMessagesHandler* userMessagesHandler);
    DialogHandler(const DialogHandler&) = delete;
    DialogHandler(DialogHandler&&) = delete;

    ~DialogHandler() override;

    DialogHandler& operator=(const DialogHandler&) = delete;
    DialogHandler& operator=(DialogHandler&&) = delete;

public: // AbstractOverwriteDialog API
    [[nodiscard]]
    Answer queryOverwrite(const QUrl& url, const QString& title) const override;

public: // AbstractSaveDiscardDialog API
    [[nodiscard]]
    Answer queryDiscardOnReload(const AbstractDocument* document, const QString& title) const override;
    [[nodiscard]]
    Answer querySaveDiscard(const AbstractDocument* document, const QString& title) const override;
    [[nodiscard]]
    Answer queryDiscard(const AbstractDocument* document, const QString& title) const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
