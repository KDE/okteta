/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewhtmlstreamencoder.hpp"

// lib
#include "oktetakastengui_export.hpp"
#include "offsetcolumnhtmlrenderer.hpp"
#include "valuebytearraycolumnhtmlrenderer.hpp"
#include "charbytearraycolumnhtmlrenderer.hpp"
#include "bytearrayrowscolumnhtmlrenderer.hpp"
#include <bytearrayview.hpp>
// Okteta gui
#include <Okteta/ByteArrayTableLayout>
#include <Okteta/AbstractByteArrayView>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/Character>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>
// Std
#include <utility>

namespace Kasten {

ByteArrayViewHtmlStreamEncoderSettings::ByteArrayViewHtmlStreamEncoderSettings() = default;

ByteArrayViewHtmlStreamEncoder::ByteArrayViewHtmlStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "View in HTML"), QStringLiteral("text/html"), QStringLiteral("text/html"))
{}

ByteArrayViewHtmlStreamEncoder::~ByteArrayViewHtmlStreamEncoder() = default;

bool ByteArrayViewHtmlStreamEncoder::encodeDataToStream(QIODevice* device,
                                                        const ByteArrayView* byteArrayView,
                                                        const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                        const Okteta::AddressRange& range)
{
    bool success = true;

    // settings
    mSettings.codecName = byteArrayView->charCodingName();
    mSettings.valueCoding = (Okteta::ValueCoding)byteArrayView->valueCoding();
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();
    mSettings.firstLineOffset = byteArrayView->firstLineOffset();
    mSettings.startOffset = byteArrayView->startOffset();
    mSettings.delta = byteArrayView->noOfBytesPerLine();
    const int viewModus = byteArrayView->viewModus();

    // setup
    Okteta::ByteArrayTableLayout layout(byteArrayView->noOfBytesPerLine(), mSettings.firstLineOffset,
                                        mSettings.startOffset, 0, byteArrayModel->size());

    Okteta::CoordRange coordRange;
    coordRange.set(layout.coordRangeOfIndizes(range));

    const int noOfBytesPerLine = byteArrayView->noOfBytesPerLine();
    const int noOfGroupedBytes = byteArrayView->noOfGroupedBytes();
    const int visibleByteArrayCodings = byteArrayView->visibleByteArrayCodings();

    QVector<AbstractColumnHtmlRenderer*> columnHtmlRendererList;

    if (byteArrayView->offsetColumnVisible()) {
        columnHtmlRendererList.append(
            new OffsetColumnHtmlRenderer(byteArrayView->offsetCoding(), mSettings.firstLineOffset, mSettings.delta, (viewModus == 0)));
    }

    if (viewModus == 0) {
        if (visibleByteArrayCodings & Okteta::AbstractByteArrayView::ValueCodingId) {
            columnHtmlRendererList.append(
                new ValueByteArrayColumnHtmlRenderer(byteArrayModel, range.start(), coordRange,
                                                     noOfBytesPerLine, noOfGroupedBytes,
                                                     mSettings.valueCoding));
        }

        if (visibleByteArrayCodings & Okteta::AbstractByteArrayView::CharCodingId) {
            columnHtmlRendererList.append(
                new CharByteArrayColumnHtmlRenderer(byteArrayModel, range.start(), coordRange,
                                                    noOfBytesPerLine,
                                                    mSettings.codecName, mSettings.substituteChar, mSettings.undefinedChar));
        }
    } else {
        columnHtmlRendererList.append(
            new ByteArrayRowsColumnHtmlRenderer(byteArrayModel, range.start(), coordRange,
                                                noOfBytesPerLine, noOfGroupedBytes,
                                                visibleByteArrayCodings,
                                                mSettings.valueCoding,
                                                mSettings.codecName, mSettings.substituteChar, mSettings.undefinedChar));
    }

    int subLinesCount = 1;
    for (const AbstractColumnHtmlRenderer* renderer : std::as_const(columnHtmlRendererList)) {
        if (renderer->noOfSublinesNeeded() > subLinesCount) {
            subLinesCount = renderer->noOfSublinesNeeded();
        }
    }

    // encode
    QTextStream textStream(device);
    textStream.setCodec("UTF-8");

    textStream << "<html><meta charset=\"utf-8\"><meta name=\"generator\" content=\"Okteta ByteArrayViewHtmlStreamEncoder " OKTETAKASTENGUI_VERSION_STRING "\"><table>" << Qt::endl;

    textStream << "<tr>";

    int l = coordRange.start().line();
    for (const AbstractColumnHtmlRenderer* renderer : std::as_const(columnHtmlRendererList)) {
        renderer->renderFirstLine(&textStream, l);
    }

    textStream << "</tr>" << Qt::endl;

    int subLine = 1;
    while (true) {
        if (subLine == subLinesCount) {
            ++l;
            if (l > coordRange.end().line()) {
                break;
            }
            subLine = 0;
        }

        textStream << "<tr>";

        const bool isSubline = (subLine > 0);
        for (const AbstractColumnHtmlRenderer* renderer : std::as_const(columnHtmlRendererList)) {
            renderer->renderNextLine(&textStream, isSubline);
        }

        textStream << "</tr>" << Qt::endl;
        ++subLine;
    }

    textStream << "</table></html>" << Qt::endl;

    // clean up
    qDeleteAll(columnHtmlRendererList);

    return success;
}

}

#include "moc_bytearrayviewhtmlstreamencoder.cpp"
