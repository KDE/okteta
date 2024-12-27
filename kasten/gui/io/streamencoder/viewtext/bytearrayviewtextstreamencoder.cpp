/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewtextstreamencoder.hpp"

// lib
#include "offsetcolumntextrenderer.hpp"
#include "bordercolumntextrenderer.hpp"
#include "valuebytearraycolumntextrenderer.hpp"
#include "charbytearraycolumntextrenderer.hpp"
#include "bytearrayrowscolumntextrenderer.hpp"
#include <bytearrayview.hpp>
// Okteta gui
#include <Okteta/ByteArrayTableLayout>
#include <Okteta/AbstractByteArrayView>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>
// Std
#include <memory>
#include <vector>

namespace Kasten {

// static constexpr Okteta::OffsetFormat::Format DefaultOffsetFormat = Okteta::OffsetFormat::Hexadecimal;

ByteArrayViewTextStreamEncoderSettings::ByteArrayViewTextStreamEncoderSettings()
    // : offsetFormat(DefaultOffsetFormat)
    : separation(QStringLiteral(" "))
{}

ByteArrayViewTextStreamEncoder::ByteArrayViewTextStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "View in Plain Text"), QStringLiteral("text/plain"))
{}

ByteArrayViewTextStreamEncoder::~ByteArrayViewTextStreamEncoder() = default;

bool ByteArrayViewTextStreamEncoder::encodeDataToStream(QIODevice* device,
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
    const int byteSpacingWidth = byteArrayView->byteSpacingWidth();
    const int noOfGroupedBytes = byteArrayView->noOfGroupedBytes();
    const int visibleByteArrayCodings = byteArrayView->visibleByteArrayCodings();

    std::vector<std::unique_ptr<AbstractColumnTextRenderer>> columnTextRendererList;

    if (byteArrayView->offsetColumnVisible()) {
        columnTextRendererList.emplace_back(
            std::make_unique<OffsetColumnTextRenderer>(byteArrayView->offsetCoding(), mSettings.firstLineOffset, mSettings.delta));
        columnTextRendererList.emplace_back(std::make_unique<BorderColumnTextRenderer>());
    }

    if (viewModus == ByteArrayView::ColumnViewId) {
        if (visibleByteArrayCodings & Okteta::AbstractByteArrayView::ValueCodingId) {
            columnTextRendererList.emplace_back(
                std::make_unique<ValueByteArrayColumnTextRenderer>(byteArrayModel, range.start(), coordRange,
                                                                   noOfBytesPerLine, byteSpacingWidth, noOfGroupedBytes,
                                                                   mSettings.valueCoding));
        }

        if (visibleByteArrayCodings & Okteta::AbstractByteArrayView::CharCodingId) {
            if (visibleByteArrayCodings & Okteta::AbstractByteArrayView::ValueCodingId) {
                columnTextRendererList.emplace_back(std::make_unique<BorderColumnTextRenderer>());
            }
            columnTextRendererList.emplace_back(
                std::make_unique<CharByteArrayColumnTextRenderer>(byteArrayModel, range.start(), coordRange,
                                                                  noOfBytesPerLine, 0, 0,
                                                                  mSettings.codecName, mSettings.substituteChar, mSettings.undefinedChar));
        }
    } else {
        columnTextRendererList.emplace_back(
            std::make_unique<ByteArrayRowsColumnTextRenderer>(byteArrayModel, range.start(), coordRange,
                                                              noOfBytesPerLine, byteSpacingWidth, noOfGroupedBytes,
                                                              visibleByteArrayCodings,
                                                              mSettings.valueCoding,
                                                              mSettings.codecName, mSettings.substituteChar, mSettings.undefinedChar));
    }

    int subLinesCount = 1;
    for (const auto& renderer : columnTextRendererList) {
        if (renderer->noOfSublinesNeeded() > subLinesCount) {
            subLinesCount = renderer->noOfSublinesNeeded();
        }
    }

    // encode
    QTextStream textStream(device);

    int l = coordRange.start().line();
    for (const auto& renderer : columnTextRendererList) {
        renderer->renderFirstLine(&textStream, l);
    }

    textStream << Qt::endl;

    int subLine = 1;
    while (true) {
        if (subLine == subLinesCount) {
            ++l;
            if (l > coordRange.end().line()) {
                break;
            }
            subLine = 0;
        }
        const bool isSubline = (subLine > 0);
        for (const auto& renderer : columnTextRendererList) {
            renderer->renderNextLine(&textStream, isSubline);
        }

        textStream << Qt::endl;
        ++subLine;
    }

    return success;
}

}

#include "moc_bytearrayviewtextstreamencoder.cpp"
