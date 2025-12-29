/*
    SPDX-FileCopyrightText: 2004, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "browserextension.hpp"

// part
#include "part.hpp"
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayViewProfileSynchronizer>
#include <Kasten/Okteta/ByteArrayView>
// KF
#include <KActionCollection>
// Qt
#include <QClipboard>
#include <QApplication>
#include <QDataStream>
#include <QMimeData>
#include <QAction>

OktetaBrowserExtension::OktetaBrowserExtension(OktetaPart* part)
    : KParts::BrowserExtension(part)
    , mPart(part)
{
    setObjectName(QStringLiteral("oktetapartbrowserextension"));

    connect(mPart, SIGNAL(hasSelectedDataChanged(bool)),
            SLOT(onSelectionChanged(bool)));

    Q_EMIT enableAction("copy", false);
    Q_EMIT enableAction("print", true);
}

void OktetaBrowserExtension::copy()
{
    auto mimeData = mPart->byteArrayView()->copySelectedData();
    if (!mimeData) {
        return;
    }

    QApplication::clipboard()->setMimeData(mimeData.release(), QClipboard::Clipboard);
}

void OktetaBrowserExtension::print()
{
    QAction* const printAction = mPart->actionCollection()->action(QStringLiteral("file_print"));
    if (printAction) {
        printAction->trigger();
    }
}

void OktetaBrowserExtension::onSelectionChanged(bool hasSelection)
{
    Q_EMIT enableAction("copy", hasSelection);
}

void OktetaBrowserExtension::saveState(QDataStream& stream)
{
    KParts::BrowserExtension::saveState(stream);

    Kasten::ByteArrayView* const view = mPart->byteArrayView();
    Kasten::ByteArrayViewProfileSynchronizer* const viewProfileSynchronizer = view->synchronizer();

    const QString viewProfileId = viewProfileSynchronizer ? viewProfileSynchronizer->viewProfileId() : QString();

    stream
        << view->zoomScale()

        << (int)view->offsetColumnVisible()
        << view->offsetCoding()
        << view->visibleByteArrayCodings()

        << (int)view->layoutStyle()
        << view->noOfBytesPerLine()
        << view->noOfGroupedBytes()

        << (int)view->valueCoding()
        << view->charCodingName()
        << (int)view->showsNonprinting()
//         << view->xOffset() << view->yOffset()
        << view->cursorPosition()
//         << (int)view->isCursorBehind()
//         << view->activeCoding()

        << view->viewModus()

        << viewProfileId
    ;
}

void OktetaBrowserExtension::restoreState(QDataStream& stream)
{
    KParts::BrowserExtension::restoreState(stream);

    double zoomScale;

    int offsetColumnVisible;
    int offsetCoding;
    int visibleCodings;

    int layoutStyle;
    int noOfBytesPerLine;
    int noOfGroupedBytes;

    int valueCoding;
    QString charCodingName;
    int showsNonprinting;
//     int x, y;
    int position;
//     int cursorBehind;
//     int activeCoding;

    int viewModus;

    QString viewProfileId;

    stream
        >> zoomScale

        >> offsetColumnVisible
        >> offsetCoding
        >> visibleCodings

        >> layoutStyle
        >> noOfBytesPerLine
        >> noOfGroupedBytes

        >> valueCoding
        >> charCodingName
        >> showsNonprinting
        >> position
//            >> cursorBehind
//            >> activeCoding
        >> viewModus
        >> viewProfileId
    ;

    Kasten::ByteArrayView* const view = mPart->byteArrayView();

    Kasten::ByteArrayViewProfileSynchronizer* const viewProfileSynchronizer = view->synchronizer();
    if (viewProfileSynchronizer) {
        viewProfileSynchronizer->setViewProfileId(viewProfileId);
    }

    view->setZoomScale(zoomScale);

    view->setViewModus(viewModus);

    view->toggleOffsetColumn(offsetColumnVisible != 0);
    view->setOffsetCoding(offsetCoding);
    view->setVisibleByteArrayCodings(visibleCodings);

    view->setLayoutStyle(layoutStyle);
    view->setNoOfBytesPerLine(noOfBytesPerLine);
    view->setNoOfGroupedBytes(noOfGroupedBytes);

    view->setValueCoding(valueCoding);
    view->setCharCoding(charCodingName);
    view->setShowsNonprinting(showsNonprinting != 0);
//     view->setColumnsPos( x, y );
    view->setCursorPosition(position);  // , cursorBehind );
//     view->setActiveCoding( (Okteta::ByteArrayColumnView::CodingTypeId)activeCoding );
}

#include "moc_browserextension.cpp"
