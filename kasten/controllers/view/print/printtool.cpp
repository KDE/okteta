/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printtool.hpp"

// controller
#include "printjob.hpp"
#include "printdialog.hpp"
#include "framestopaperprinter.hpp"
#include "headerfooterframerenderer.hpp"
#include "bytearrayframerenderer.hpp"
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
    emit viewChanged(hasView);
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
    connect(printDialog, QOverload<QPrinter*>::of(&QPrintDialog::accepted), this, &PrintTool::triggerPrint);
    printDialog->open();
}

void PrintTool::triggerPrint(QPrinter* printer)
{
    const QString creator = QStringLiteral("Print Plugin for Okteta " OKTETA_VERSION);   // no i18n(), keep space at end as separator
    printer->setCreator(creator);

    FramesToPaperPrinter framesPrinter;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    const QPageLayout pageLayout = printer->pageLayout();
    const int printerResolution = printer->resolution();
    framesPrinter.setPaperRect(pageLayout.fullRectPixels(printerResolution));
    framesPrinter.setPageRect(pageLayout.paintRectPixels(printerResolution));
#else
    framesPrinter.setPaperRect(printer->paperRect());
    framesPrinter.setPageRect(printer->pageRect());
#endif
    printer->setFullPage(true);

    PrintInfo info;
    const QRect pageRect = framesPrinter.pageRect();
    const int left = pageRect.left();
    const int width = pageRect.width();

    auto* headerFrameRenderer = new HeaderFooterFrameRenderer(&info);
    headerFrameRenderer->setTexts(QStringLiteral("%d"),
                                    QStringLiteral("%f"),
                                    i18nc("in the header of the printed page, e.g. Page 2 of 20", "Page %p of %P"));
    headerFrameRenderer->setWidth(width);
    headerFrameRenderer->setPos(pageRect.topLeft());

    auto* footerFrameRenderer = new HeaderFooterFrameRenderer(&info);
    footerFrameRenderer->setTexts(i18nc("in the footer of the printed page, e.g. Printed by: Joe User",
                                        "Printed by: %U"),
                                    QString(),
                                    QStringLiteral("%F"));
    footerFrameRenderer->setWidth(width);
    const int footerTop = pageRect.bottom() - footerFrameRenderer->height();
    footerFrameRenderer->setPos(left, footerTop);

    const int contentHeight = pageRect.height() - footerFrameRenderer->height() - headerFrameRenderer->height();
    const int contentTop = pageRect.top() + headerFrameRenderer->height();
    auto* byteArrayFrameRenderer = new ByteArrayFrameRenderer;
    byteArrayFrameRenderer->setPos(left, contentTop);
    byteArrayFrameRenderer->setWidth(width);
    byteArrayFrameRenderer->setHeight(contentHeight);

    Okteta::AddressRange range = mByteArrayView->selection();
    if (!range.isValid()) {
        range.setByWidth(0, mByteArrayModel->size());
    }
    byteArrayFrameRenderer->setByteArrayModel(mByteArrayModel, range.start(), range.width());

    // TODO: use noOfBytesPerLine of view, scale resolution down if it does not fit the page
    const int noOfBytesPerLine = mByteArrayView->noOfBytesPerLine();
//         byteArrayFrameRenderer->setNoOfBytesPerLine( mByteArrayView->noOfBytesPerLine() );

    const Okteta::Address startOffset = mByteArrayView->startOffset() + range.start();
    const int line = startOffset / noOfBytesPerLine;
    const Okteta::Address firstLineOffset = mByteArrayView->firstLineOffset() + line * noOfBytesPerLine;
    byteArrayFrameRenderer->setFirstLineOffset(firstLineOffset);
    byteArrayFrameRenderer->setStartOffset(startOffset);

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

//     if( !confirmPrintPageNumber( byteArrayFrameRenderer->framesCount()) )
//         return;

    framesPrinter.addFrameRenderer(headerFrameRenderer);
    framesPrinter.addFrameRenderer(byteArrayFrameRenderer);
    framesPrinter.addFrameRenderer(footerFrameRenderer);

    info.setNoOfPages(byteArrayFrameRenderer->framesCount());
    AbstractModelSynchronizer* synchronizer = mDocument->synchronizer();
    if (synchronizer) {
        info.setUrl(synchronizer->url());
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* printJob = new PrintJob(&framesPrinter, 0, byteArrayFrameRenderer->framesCount() - 1, printer);
    const bool success = printJob->exec();

    delete printer;

    QApplication::restoreOverrideCursor();

    if (!success) {
        const QString message = i18nc("@info", "Could not print.");

        KMessageBox::sorry(QApplication::activeWindow(), message, i18nc("@title:window", "Print Byte Array %1", mDocument->title()));
    }
}

}
