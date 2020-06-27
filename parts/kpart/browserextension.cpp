/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "browserextension.hpp"

// part
#include "part.hpp"
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayViewProfileSynchronizer>
#include <Kasten/Okteta/ByteArrayView>
#include <Kasten/Okteta/ByteArrayDocument>
// KF
#include <KActionCollection>
// Qt
#include <QClipboard>
#include <QApplication>
#include <QDataStream>
#include <QAction>

OktetaBrowserExtension::OktetaBrowserExtension(OktetaPart* part)
    : KParts::BrowserExtension(part)
    , mPart(part)
{
    setObjectName(QStringLiteral("oktetapartbrowserextension"));

    connect(mPart, SIGNAL(hasSelectedDataChanged(bool)),
            SLOT(onSelectionChanged(bool)));

    emit enableAction("copy", false);
    emit enableAction("print", true);
}

void OktetaBrowserExtension::copy()
{
    QMimeData* data = mPart->byteArrayView()->copySelectedData();
    if (!data) {
        return;
    }

    QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
}

void OktetaBrowserExtension::print()
{
    auto printAction = mPart->actionCollection()->action(QStringLiteral("file_print"));
    if (printAction) {
        printAction->trigger();
    }
}

void OktetaBrowserExtension::onSelectionChanged(bool hasSelection)
{
    emit enableAction("copy", hasSelection);
}

void OktetaBrowserExtension::saveState(QDataStream& stream)
{
    KParts::BrowserExtension::saveState(stream);

    Kasten::ByteArrayView* view = mPart->byteArrayView();
    Kasten::ByteArrayViewProfileSynchronizer* viewProfileSynchronizer = view->synchronizer();

    const QString viewProfileId = viewProfileSynchronizer ? viewProfileSynchronizer->viewProfileId() : QString();

    stream
        << view->zoomLevel()

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

    double zoomLevel;

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
        >> zoomLevel

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

    Kasten::ByteArrayView* view = mPart->byteArrayView();

    Kasten::ByteArrayViewProfileSynchronizer* viewProfileSynchronizer = view->synchronizer();
    if (viewProfileSynchronizer) {
        viewProfileSynchronizer->setViewProfileId(viewProfileId);
    }

    view->setZoomLevel(zoomLevel);

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
