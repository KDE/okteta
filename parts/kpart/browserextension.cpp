/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2004,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "browserextension.hpp"

// part
#include "part.hpp"
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayViewProfileSynchronizer>
#include <Kasten/Okteta/ByteArrayView>
#include <Kasten/Okteta/ByteArrayDocument>
// KF5
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

    view->toggleOffsetColumn(offsetColumnVisible);
    view->setOffsetCoding(offsetCoding);
    view->setVisibleByteArrayCodings(visibleCodings);

    view->setLayoutStyle(layoutStyle);
    view->setNoOfBytesPerLine(noOfBytesPerLine);
    view->setNoOfGroupedBytes(noOfGroupedBytes);

    view->setValueCoding(valueCoding);
    view->setCharCoding(charCodingName);
    view->setShowsNonprinting(showsNonprinting);
//     view->setColumnsPos( x, y );
    view->setCursorPosition(position);  // , cursorBehind );
//     view->setActiveCoding( (Okteta::ByteArrayColumnView::CodingTypeId)activeCoding );
}
