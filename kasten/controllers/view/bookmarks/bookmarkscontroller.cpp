/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkscontroller.hpp"

// controller
#include "bookmarkeditpopup.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Kasten core
#include <Kasten/AbstractModel>
// Okteta gui
#include <Okteta/OffsetFormat>
// Okteta core
#include <Okteta/TextByteArrayAnalyzer>
#include <Okteta/CharCodec>
#include <Okteta/Bookmarkable>
#include <Okteta/BookmarksConstIterator>
#include <Okteta/Bookmark>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QActionGroup>
#include <QAction>

namespace Kasten {

static constexpr char BookmarkListActionListId[] = "bookmark_list";

// TODO: Sortieren nach Offset oder Zeit

BookmarksController::BookmarksController(KXMLGUIClient* guiClient)
    : mGuiClient(guiClient)
{
    KActionCollection* actionCollection = mGuiClient->actionCollection();

    mCreateAction = KStandardAction::addBookmark(this, &BookmarksController::createBookmark, this);
    mCreateAction->setEnabled(false);

    mDeleteAction = new QAction(QIcon::fromTheme(QStringLiteral("bookmark-remove")),
                                i18nc("@action:inmenu", "Remove Bookmark"), this);
    mDeleteAction->setObjectName(QStringLiteral("bookmark_remove"));
    connect(mDeleteAction, &QAction::triggered, this, &BookmarksController::deleteBookmark);
    actionCollection->setDefaultShortcut(mDeleteAction, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_B));
    mDeleteAction->setEnabled(false);

    mDeleteAllAction = new QAction(QIcon::fromTheme(QStringLiteral("bookmark-remove")),
                                   i18nc("@action:inmenu", "Remove All Bookmarks"), this);
    mDeleteAllAction->setObjectName(QStringLiteral("bookmark_remove_all"));
    connect(mDeleteAllAction, &QAction::triggered, this, &BookmarksController::deleteAllBookmarks);
    mDeleteAllAction->setEnabled(false);
//     actionCollection->setDefaultShortcut(mDeleteAllAction, QKeySequence(Qt::CTRL | Qt::Key_G));

    mGotoNextBookmarkAction = new QAction(QIcon::fromTheme(QStringLiteral("go-next")),
                                          i18nc("@action:inmenu", "Go to Next Bookmark"), this);
    mGotoNextBookmarkAction->setObjectName(QStringLiteral("bookmark_next"));
    connect(mGotoNextBookmarkAction, &QAction::triggered, this, &BookmarksController::gotoNextBookmark);
    actionCollection->setDefaultShortcut(mGotoNextBookmarkAction, QKeySequence(Qt::ALT | Qt::Key_PageDown));
    mGotoNextBookmarkAction->setEnabled(false);

    mGotoPreviousBookmarkAction = new QAction(QIcon::fromTheme(QStringLiteral("go-previous")),
                                              i18nc("@action:inmenu", "Go to Previous Bookmark"), this);
    mGotoPreviousBookmarkAction->setObjectName(QStringLiteral("bookmark_previous"));
    connect(mGotoPreviousBookmarkAction, &QAction::triggered, this, &BookmarksController::gotoPreviousBookmark);
    actionCollection->setDefaultShortcut(mGotoPreviousBookmarkAction, QKeySequence(Qt::ALT | Qt::Key_PageUp));
    mGotoPreviousBookmarkAction->setEnabled(false);

    mBookmarksActionGroup = new QActionGroup(this);   // TODO: do we use this only for the signal mapping?
//     mBookmarksActionGroup->setExclusive( true );
    connect(mBookmarksActionGroup, &QActionGroup::triggered,
            this, &BookmarksController::onBookmarkTriggered);

    actionCollection->addActions({
        mCreateAction,
        mDeleteAction,
        mDeleteAllAction,
        mGotoNextBookmarkAction,
        mGotoPreviousBookmarkAction
    });
}

BookmarksController::~BookmarksController() = default;

void BookmarksController::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }
    if (mByteArray) {
        mByteArray->disconnect(this);
    }

    mByteArrayView = byteArrayView;

    // TODO; check if already same document used before
    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArray = document ? document->content() : nullptr;
    mBookmarks = (mByteArray && mByteArrayView) ? qobject_cast<Okteta::Bookmarkable*>(mByteArray) : nullptr;

    const bool hasViewWithBookmarks = (mBookmarks != nullptr);
    int bookmarksCount = 0;
    if (hasViewWithBookmarks) {
        bookmarksCount = mBookmarks->bookmarksCount();
        connect(mByteArray, SIGNAL(bookmarksAdded(QVector<Okteta::Bookmark>)),
                SLOT(onBookmarksAdded(QVector<Okteta::Bookmark>)));
        connect(mByteArray, SIGNAL(bookmarksRemoved(QVector<Okteta::Bookmark>)),
                SLOT(onBookmarksRemoved(QVector<Okteta::Bookmark>)));
        connect(mByteArray, SIGNAL(bookmarksModified(QVector<int>)),
                SLOT(updateBookmarks()));
        connect(mByteArrayView, &ByteArrayView::cursorPositionChanged,
                this, &BookmarksController::onCursorPositionChanged);
        connect(mByteArrayView, &ByteArrayView::offsetCodingChanged,
                this, &BookmarksController::updateBookmarks);
    }

    updateBookmarks();

    const bool hasBookmarks = hasViewWithBookmarks && (bookmarksCount != 0);
    if (hasViewWithBookmarks) {
        onCursorPositionChanged(mByteArrayView->cursorPosition());
    } else {
        mCreateAction->setEnabled(false);
        mDeleteAction->setEnabled(false);
        mGotoNextBookmarkAction->setEnabled(false);
        mGotoPreviousBookmarkAction->setEnabled(false);
    }
    mDeleteAllAction->setEnabled(hasBookmarks);
}

void BookmarksController::updateBookmarks()
{
    mGuiClient->unplugActionList(QLatin1String(BookmarkListActionListId));

    qDeleteAll(mBookmarksActionGroup->actions());

    if (!mBookmarks) {
        return;
    }

    const Okteta::Address startOffset = mByteArrayView->startOffset();
    Okteta::OffsetFormat::print printFunction =
        Okteta::OffsetFormat::printFunction((Okteta::OffsetFormat::Format)mByteArrayView->offsetCoding());

    char codedOffset[Okteta::OffsetFormat::MaxFormatWidth + 1];

    constexpr int firstWithNumericShortCut = 1;
    constexpr int lastWithNumericShortCut = 9;
    int b = firstWithNumericShortCut;

    Okteta::BookmarksConstIterator bit = mBookmarks->createBookmarksConstIterator();
    while (bit.hasNext()) {
        const Okteta::Bookmark& bookmark = bit.next();
        printFunction(codedOffset, startOffset + bookmark.offset());
        QString title = i18nc("@item description of bookmark", "%1: %2", QString::fromUtf8(codedOffset), bookmark.name());
        if (b <= lastWithNumericShortCut) {
            title = QStringLiteral("&%1 %2").arg(b).arg(title);
            // = KStringHandler::rsqueeze( view->title(), MaxEntryLength );
            ++b;
        }
        auto* action = new QAction(title, mBookmarksActionGroup);

        action->setData(bookmark.offset());
        mBookmarksActionGroup->addAction(action);
    }
    mGuiClient->plugActionList(QString::fromUtf8(BookmarkListActionListId),
                               mBookmarksActionGroup->actions());
}

void BookmarksController::onBookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks)
{
    Q_UNUSED(bookmarks)
    const Okteta::Address currentPosition = mByteArrayView->cursorPosition();
    onCursorPositionChanged(currentPosition);

    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = (bookmarksCount != 0);

    mDeleteAllAction->setEnabled(hasBookmarks);

    updateBookmarks();
}

void BookmarksController::onBookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks)
{
    Q_UNUSED(bookmarks)
    const Okteta::Address currentPosition = mByteArrayView->cursorPosition();
    onCursorPositionChanged(currentPosition);

    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = (bookmarksCount != 0);

    mDeleteAllAction->setEnabled(hasBookmarks);

    updateBookmarks();
}

void BookmarksController::onCursorPositionChanged(Okteta::Address newPosition)
{
    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = (bookmarksCount != 0);
    const bool isInsideByteArray = (newPosition < mByteArray->size());
    bool isAtBookmark;
    bool hasPrevious;
    bool hasNext;
    if (hasBookmarks) {
        isAtBookmark = mBookmarks->containsBookmarkFor(newPosition);
        Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
        hasPrevious = bookmarksIterator.findPreviousFrom(newPosition - 1);
        hasNext = bookmarksIterator.findNextFrom(newPosition + 1);
    } else {
        isAtBookmark = false;
        hasPrevious = false;
        hasNext = false;
    }

    mCreateAction->setEnabled(!isAtBookmark && isInsideByteArray);
    mDeleteAction->setEnabled(isAtBookmark);
    mGotoNextBookmarkAction->setEnabled(hasNext);
    mGotoPreviousBookmarkAction->setEnabled(hasPrevious);
}

void BookmarksController::createBookmark()
{
    const Okteta::Address cursorPosition = mByteArrayView->cursorPosition();

    // search for text at cursor
    std::unique_ptr<const Okteta::CharCodec> charCodec(Okteta::CharCodec::createCodec(mByteArrayView->charCodingName()));
    const Okteta::TextByteArrayAnalyzer textAnalyzer(mByteArray, charCodec.get());
    QString bookmarkName = textAnalyzer.text(cursorPosition, cursorPosition + MaxBookmarkNameSize - 1);
    charCodec.reset();

    if (bookmarkName.isEmpty()) {
        bookmarkName = i18nc("default name of a bookmark", "Bookmark");  // %1").arg( 0 ) ); // TODO: use counter like with new file, globally

    }
    auto* bookmarkEditPopup = new BookmarkEditPopup(mByteArrayView->widget());
    QPoint popupPoint = mByteArrayView->cursorRect().topLeft();
//     popupPoint.ry() += mSlider->height() / 2;
    popupPoint = mByteArrayView->widget()->mapToGlobal(popupPoint);

    bookmarkEditPopup->setPosition(popupPoint);
    bookmarkEditPopup->setName(bookmarkName);
    bookmarkEditPopup->setCursorPosition(cursorPosition);
    connect(bookmarkEditPopup, &BookmarkEditPopup::bookmarkAccepted,
            this, &BookmarksController::addBookmark);
    bookmarkEditPopup->open();
}

void BookmarksController::addBookmark(Okteta::Address cursorPosition, const QString& name)
{
    Okteta::Bookmark bookmark(cursorPosition);
    bookmark.setName(name);

    const QVector<Okteta::Bookmark> bookmarks { bookmark };
    mBookmarks->addBookmarks(bookmarks);
}

void BookmarksController::deleteBookmark()
{
    const Okteta::Address cursorPosition = mByteArrayView->cursorPosition();
    const QVector<Okteta::Bookmark> bookmarks { cursorPosition };
    mBookmarks->removeBookmarks(bookmarks);
}

void BookmarksController::deleteAllBookmarks()
{
    mBookmarks->removeAllBookmarks();
}

void BookmarksController::gotoNextBookmark()
{
    const Okteta::Address positionAfter = mByteArrayView->cursorPosition() + 1;

    Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
    const bool hasNext = bookmarksIterator.findNextFrom(positionAfter);
    if (hasNext) {
        const Okteta::Address newPosition = bookmarksIterator.next().offset();
        mByteArrayView->setCursorPosition(newPosition);
    }
}

void BookmarksController::gotoPreviousBookmark()
{
    const Okteta::Address positionBefore = mByteArrayView->cursorPosition() - 1;

    Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
    const bool hasPrevious = bookmarksIterator.findPreviousFrom(positionBefore);
    if (hasPrevious) {
        const Okteta::Address newPosition = bookmarksIterator.previous().offset();
        mByteArrayView->setCursorPosition(newPosition);
    }
}

void BookmarksController::onBookmarkTriggered(QAction* action)
{
    const Okteta::Address newPosition = action->data().toInt();
    mByteArrayView->setCursorPosition(newPosition);
}

}

#include "moc_bookmarkscontroller.cpp"
