/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkeditpopup.hpp"

// Okteta core
// #include <Okteta/Bookmark>
// Qt
#include <QLineEdit>
#include <QIcon>

namespace Kasten {

BookmarkEditPopup::BookmarkEditPopup(QWidget* parent)
    : AbstractLinePopup(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    setIcon(QIcon::fromTheme(QStringLiteral("bookmark-new")));

    mBookmarkNameLineEdit = new QLineEdit(this);
    mBookmarkNameLineEdit->setClearButtonEnabled(true);
    connect(mBookmarkNameLineEdit, &QLineEdit::returnPressed, this, &AbstractLinePopup::accept);

    setWidget(mBookmarkNameLineEdit);

    connect(this, &AbstractLinePopup::finished, this, &BookmarkEditPopup::onFinished);
}

BookmarkEditPopup::~BookmarkEditPopup() = default;

QString BookmarkEditPopup::name() const { return mBookmarkNameLineEdit->text(); }

void BookmarkEditPopup::setName(const QString& name)
{
    mBookmarkNameLineEdit->setText(name);
    mBookmarkNameLineEdit->selectAll();
}

void BookmarkEditPopup::setCursorPosition(int cursorPosition)
{
    m_cursorPosition = cursorPosition;
}

void BookmarkEditPopup::onFinished(int result)
{
    if (result != Accepted) {
        return;
    }

    emit bookmarkAccepted(m_cursorPosition, name());
}

}
