/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printtool.hpp"

// controller
#include "printjob.hpp"
#include "printdialog.hpp"
#include "printpreviewdialog.hpp"
#include "framestopaperprinter.hpp"
#include "headerfooterframerenderer.hpp"
#include "bytearraycolumnframerenderer.hpp"
#include "bytearrayrowframerenderer.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Kasten core
#include <Kasten/AbstractModelSynchronizer>
// KF
#include <KLocalizedString>
#include <KMessageBox>
// Qt
#include <QApplication>
#include <QPrinter>
#include <QPageLayout>
#include <QFont>
#include <QFontMetrics>

namespace Kasten {

PrintTool::PrintTool() = default;

PrintTool::~PrintTool() = default;

void PrintTool::setTargetModel(AbstractModel* model)
{
//     if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    mDocument = mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = mDocument ? mDocument->content() : nullptr;

    const bool hasView = (mByteArrayView && mByteArrayModel);
    Q_EMIT viewChanged(hasView);
}

void PrintTool::print()
{
    auto* printer = new QPrinter;

//     LayoutDialogPage* layoutPage = new LayoutDialogPage();
//     QList<QWidget*> customDialogPages;
//     customDialogPages << layoutPage;
    auto* printDialog = new PrintDialog(printer, QApplication::activeWindow());
    // Disable PrintPageRange, this tells Qt we can't do client-side page selection,
    // so it will try do server-side page selection if supported
    printDialog->setOption(QPrintDialog::PrintPageRange, false);
//    printDialog->setOptionTabs(customDialogPages);

    printDialog->setWindowTitle(i18nc("@title:window", "Print Byte Array %1", mDocument->title()));
    connect(printDialog, qOverload<QPrinter*>(&QPrintDialog::accepted), this, &PrintTool::triggerPrint);
    printDialog->open();
}

void PrintTool::printPreview()
{
    auto* printer = new QPrinter;

    auto* previewDialog = new PrintPreviewDialog(printer, QApplication::activeWindow());

    connect(previewDialog, &QPrintPreviewDialog::paintRequested,
            this, &PrintTool::triggerPrintPreview);
    previewDialog->show();
}

void PrintTool::triggerPrint(QPrinter* printer)
{
    const bool success = doPrint(printer);

    delete printer;

    if (!success) {
        const QString message = i18nc("@info", "Could not print.");

        KMessageBox::error(QApplication::activeWindow(), message, i18nc("@title:window", "Print Byte Array %1", mDocument->title()));
    }
}

void PrintTool::triggerPrintPreview(QPrinter* printer)
{
    doPrint(printer);
}

bool PrintTool::doPrint(QPrinter* printer)
{
    const QString creator = QStringLiteral("Print Plugin for Okteta " OKTETA_VERSION);   // no i18n(), keep space at end as separator
    printer->setCreator(creator);

    FramesToPaperPrinter framesPrinter;
    const QPageLayout pageLayout = printer->pageLayout();
    const int printerResolution = printer->resolution();
    framesPrinter.setPaperRect(pageLayout.fullRectPixels(printerResolution));
    framesPrinter.setPageRect(pageLayout.paintRectPixels(printerResolution));
    framesPrinter.setPageMargins(pageLayout.marginsPixels(printerResolution));
    printer->setFullPage(true);

    PrintInfo info;
    const QRect pageRect = framesPrinter.contentsRect();
    const int left = pageRect.left();
    const int width = pageRect.width();

    auto headerFrameRenderer = std::make_unique<HeaderFooterFrameRenderer>(&info);
    headerFrameRenderer->setTexts(QStringLiteral("%d"),
                                    QStringLiteral("%f"),
                                    i18nc("in the header of the printed page, e.g. Page 2 of 20", "Page %p of %P"));
    headerFrameRenderer->setWidth(width);
    headerFrameRenderer->setPos(pageRect.topLeft());

    auto footerFrameRenderer = std::make_unique<HeaderFooterFrameRenderer>(&info);
    footerFrameRenderer->setTexts(i18nc("in the footer of the printed page, e.g. Printed by: Joe User",
                                        "Printed by: %U"),
                                    QString(),
                                    QStringLiteral("%F"));
    footerFrameRenderer->setWidth(width);
    const int footerTop = pageRect.bottom() - footerFrameRenderer->height();
    footerFrameRenderer->setPos(left, footerTop);

    const int contentHeight = pageRect.height() - footerFrameRenderer->height() - headerFrameRenderer->height();
    const int contentTop = pageRect.top() + headerFrameRenderer->height();
    auto byteArrayFrameRenderer = (mByteArrayView->viewModus() == ByteArrayView::ColumnViewId) ?
        std::unique_ptr<AbstractByteArrayFrameRenderer>(std::make_unique<ByteArrayColumnFrameRenderer>()) :
        std::unique_ptr<AbstractByteArrayFrameRenderer>(std::make_unique<ByteArrayRowFrameRenderer>());
    byteArrayFrameRenderer->setPos(left, contentTop);
    byteArrayFrameRenderer->setWidth(width);
    byteArrayFrameRenderer->setHeight(contentHeight);

    Okteta::AddressRange range = mByteArrayView->selection();
    if (!range.isValid()) {
        range.setByWidth(0, mByteArrayModel->size());
    }
    byteArrayFrameRenderer->setByteArrayModel(mByteArrayModel, range.start(), range.width());

    // TODO: scale resolution down if it does not fit the page
    const int noOfBytesPerLine = mByteArrayView->noOfBytesPerLine();
    byteArrayFrameRenderer->setNoOfBytesPerLine(mByteArrayView->noOfBytesPerLine());

    const Okteta::Address startOffset = mByteArrayView->startOffset() + range.start();
    const int line = startOffset / noOfBytesPerLine;
    const Okteta::Address firstLineOffset = mByteArrayView->firstLineOffset() + line * noOfBytesPerLine;
    byteArrayFrameRenderer->setFirstLineOffset(firstLineOffset);
    byteArrayFrameRenderer->setStartOffset(startOffset);
    byteArrayFrameRenderer->setOffsetCoding((Okteta::OffsetFormat::Format)mByteArrayView->offsetCoding());

    byteArrayFrameRenderer->setCharCoding(mByteArrayView->charCodingName());
    byteArrayFrameRenderer->setBufferSpacing(mByteArrayView->byteSpacingWidth(),
                                                mByteArrayView->noOfGroupedBytes(),
                                                mByteArrayView->groupSpacingWidth());
    byteArrayFrameRenderer->setBinaryGapWidth(mByteArrayView->binaryGapWidth());

    byteArrayFrameRenderer->setValueCoding((Okteta::ValueCoding)mByteArrayView->valueCoding());
    byteArrayFrameRenderer->setShowsNonprinting(mByteArrayView->showsNonprinting());
    byteArrayFrameRenderer->setSubstituteChar(mByteArrayView->substituteChar());
    byteArrayFrameRenderer->setUndefinedChar(mByteArrayView->undefinedChar());
    byteArrayFrameRenderer->showByteArrayColumns(mByteArrayView->visibleByteArrayCodings());

    const int framesCount = byteArrayFrameRenderer->framesCount();
//     if( !confirmPrintPageNumber( byteArrayFrameRenderer->framesCount()) )
//         return;

    framesPrinter.addFrameRenderer(std::move(headerFrameRenderer));
    framesPrinter.addFrameRenderer(std::move(byteArrayFrameRenderer));
    framesPrinter.addFrameRenderer(std::move(footerFrameRenderer));

    info.setNoOfPages(framesCount);
    AbstractModelSynchronizer* synchronizer = mDocument->synchronizer();
    if (synchronizer) {
        info.setUrl(synchronizer->url());
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* printJob = new PrintJob(&framesPrinter, 0, framesCount - 1, printer);
    const bool success = printJob->exec();

    QApplication::restoreOverrideCursor();

    return success;
}

}

#include "moc_printtool.cpp"
