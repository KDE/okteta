/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKEDITPOPUP_HPP
#define KASTEN_BOOKMARKEDITPOPUP_HPP

// Kasten gui
#include <Kasten/AbstractLinePopup>
// Okteta core
#include <Okteta/Address>

// class Bookmark;
class QLineEdit;

namespace Kasten {

class BookmarkEditPopup : public AbstractLinePopup
{
    Q_OBJECT

public:
    explicit BookmarkEditPopup(QWidget* parent = nullptr);
    ~BookmarkEditPopup() override;

public:
    QString name() const;

public:
    void setName(const QString& name);
    void setCursorPosition(Okteta::Address cursorPosition);

Q_SIGNALS:
    void bookmarkAccepted(Okteta::Address cursorPosition, const QString& name);

private Q_SLOTS:
    void onFinished(int result);

private:
    QLineEdit* mBookmarkNameLineEdit;
    Okteta::Address m_cursorPosition;
};

}

#endif
