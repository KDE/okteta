/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printtool.hpp"
#include "printtool-config.hpp"

// controller
#include "printjob.hpp"
#include "printdialog.hpp"
#include "printpreviewdialog.hpp"
#include "bytearraycolumnframerenderer.hpp"
#include "bytearrayrowframerenderer.hpp"
// framesprint
#include <framestopaperprinter.hpp>
#include <headerfooterframerenderer.hpp>
#include <printinfo.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Kasten core
#include <Kasten/AbstractModelSynchronizer>
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserErrorReport>
// KF
#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>
// Qt
#include <QApplication>
#include <QPrinter>
// Std
#include <utility>

namespace Kasten {

// in sync with QPageLayout::Unit & QPageSize::Unit
enum PageUnit {
    PageMillimeter,
    PagePoint,
    PageInch,
    PagePica,
    PageDidot,
    PageCicero
};

static
PageUnit pageUnitFromString(const QString& string, PageUnit defaultUnit)
{
    return
        (string == QLatin1String("Millimeter")) ? PageMillimeter :
        (string == QLatin1String("Point")) ?      PagePoint :
        (string == QLatin1String("Inch")) ?       PageInch :
        (string == QLatin1String("Pica")) ?       PagePica :
        (string == QLatin1String("Didot")) ?      PageDidot :
        (string == QLatin1String("Cicero")) ?     PageCicero :
        /* else */                                defaultUnit;
}

static
QString pageUnitAsString(PageUnit unit)
{
    return
        (unit == PagePoint) ?  QStringLiteral("Point") :
        (unit == PageInch) ?   QStringLiteral("Inch") :
        (unit == PagePica) ?   QStringLiteral("Pica") :
        (unit == PageDidot) ?  QStringLiteral("Didot") :
        (unit == PageCicero) ? QStringLiteral("Cicero") :
                               QStringLiteral("Millimeter");
}

template <typename T>
static
T pageUnitFromString(const QString& string, T defaultUnit)
{
    return static_cast<T>(pageUnitFromString(string, static_cast<PageUnit>(defaultUnit)));
}

template <typename T>
static
QString pageUnitAsString(T unit)
{
    return pageUnitAsString(static_cast<PageUnit>(unit));;
}

}

template <>
inline QPageLayout::Unit KConfigGroup::readEntry(const char *key, const QPageLayout::Unit &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const QPageLayout::Unit unit = Kasten::pageUnitFromString<QPageLayout::Unit>(entry, defaultValue);
    return unit;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const QPageLayout::Unit &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString = Kasten::pageUnitAsString<QPageLayout::Unit>(value);
    writeEntry(key, valueString, flags);
}

template <>
inline QPageSize::Unit KConfigGroup::readEntry(const char *key, const QPageSize::Unit &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const QPageSize::Unit unit = Kasten::pageUnitFromString<QPageSize::Unit>(entry, defaultValue);
    return unit;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const QPageSize::Unit &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString = Kasten::pageUnitAsString<QPageSize::Unit>(value);
    writeEntry(key, valueString, flags);
}

template <>
inline QPageLayout::Orientation KConfigGroup::readEntry(const char *key, const QPageLayout::Orientation &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const QPageLayout::Orientation orientation =
        (entry == QLatin1String("Portrait")) ?  QPageLayout::Portrait :
        (entry == QLatin1String("Landscape")) ? QPageLayout::Landscape :
        /* else */                             defaultValue;
    return orientation;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const QPageLayout::Orientation &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == QPageLayout::Portrait) ? QLatin1String("Portrait") : QLatin1String("Landscape");
    writeEntry(key, valueString, flags);
}

template <>
inline QPageSize::PageSizeId KConfigGroup::readEntry(const char *key, const QPageSize::PageSizeId &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    if (!entry.isEmpty()) {
        for (int i = 0; i < QPageSize::LastPageSize; ++i) {
            const auto id = static_cast<QPageSize::PageSizeId>(i);
            const QString pageSizekey = QPageSize::key(id);
            if (pageSizekey == entry) {
                return id;
            }
        }
    }
    return defaultValue;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const QPageSize::PageSizeId &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString = QPageSize::key(value);
    writeEntry(key, valueString, flags);
}

namespace Kasten {

PrintTool::PrintTool(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{
}

PrintTool::~PrintTool() = default;

void PrintTool::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }
//     if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = byteArrayView;

    mDocument = mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = mDocument ? mDocument->content() : nullptr;

    const bool hasView = (mByteArrayView && mByteArrayModel);
    Q_EMIT viewChanged(hasView);
}

void PrintTool::loadConfig(QPrinter* printer) const
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    QPageLayout pageLayout = printer->pageLayout();

    const QPageLayout::Unit pageUnit = configGroup.readEntry(PageLayoutUnitConfigKey, DefaultPageLayoutUnit);
    pageLayout.setUnits(pageUnit);

    const qreal leftMargin = configGroup.readEntry(PageMarginLeftConfigKey, DefaultPageMarginLeft);
    const qreal rightMargin = configGroup.readEntry(PageMarginRightConfigKey, DefaultPageMarginRight);
    const qreal topMargin = configGroup.readEntry(PageMarginTopConfigKey, DefaultPageMarginTop);
    const qreal bottomMargin = configGroup.readEntry(PageMarginBottomConfigKey, DefaultPageMarginBottom);
    const QMarginsF margins = QMarginsF(leftMargin, topMargin, rightMargin, bottomMargin);
    pageLayout.setMargins(margins);

    const QPageLayout::Orientation pageOrientation =  configGroup.readEntry(PageOrientationConfigKey, DefaultPageOrientation);
    pageLayout.setOrientation(pageOrientation);

    const QPageSize::PageSizeId pageSizeId = configGroup.readEntry(PageSizeIdConfigKey, DefaultPageSizeId);
    QPageSize pageSize;
    if (pageSizeId == QPageSize::Custom) {
        const QPageSize::Unit pageSizeUnit = configGroup.readEntry(PageSizeUnitConfigKey, DefaultPageSizeUnit);
        const QSizeF defaultCustomSize = QPageSize::definitionSize(DefaultPageSizeId);
        const qreal pageWidth = configGroup.readEntry(PageWidthConfigKey, defaultCustomSize.width());
        const qreal pageHeight = configGroup.readEntry(PageHeightConfigKey, defaultCustomSize.height());
        pageSize = QPageSize(QSizeF(pageWidth, pageHeight), pageSizeUnit, QString(), QPageSize::ExactMatch);
    } else {
        pageSize = QPageSize(pageSizeId);
    }
    pageLayout.setPageSize(pageSize);

    printer->setPageLayout(pageLayout);
}

void PrintTool::storeConfig(const QPrinter* printer)
{
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    const QPageLayout pageLayout = printer->pageLayout();

    const QPageLayout::Unit layoutUnit = pageLayout.units();
    configGroup.writeEntry(PageLayoutUnitConfigKey, layoutUnit);

    const QMarginsF margins = pageLayout.margins();
    configGroup.writeEntry(PageMarginLeftConfigKey, margins.left());
    configGroup.writeEntry(PageMarginRightConfigKey, margins.right());
    configGroup.writeEntry(PageMarginTopConfigKey, margins.top());
    configGroup.writeEntry(PageMarginBottomConfigKey, margins.bottom());

    const QPageLayout::Orientation orientation = pageLayout.orientation();
    configGroup.writeEntry(PageOrientationConfigKey, orientation);

    const QPageSize pageSize = pageLayout.pageSize();

    const QPageSize::PageSizeId pageSizeId = pageSize.id();
    configGroup.writeEntry(PageSizeIdConfigKey, pageSizeId);

    if (pageSizeId == QPageSize::Custom) {
        const QPageSize::Unit sizeUnit = pageSize.definitionUnits();
        configGroup.writeEntry(PageSizeUnitConfigKey, sizeUnit);
        const QSizeF definitionSize =  pageSize.definitionSize();
        configGroup.writeEntry(PageWidthConfigKey, definitionSize.width());
        configGroup.writeEntry(PageHeightConfigKey, definitionSize.height());
    } else {
        configGroup.deleteEntry(PageSizeUnitConfigKey);
        configGroup.deleteEntry(PageWidthConfigKey);
        configGroup.deleteEntry(PageHeightConfigKey);
    }
}

void PrintTool::print()
{
    auto* const printer = new QPrinter;

    loadConfig(printer);

//     LayoutDialogPage* layoutPage = new LayoutDialogPage();
//     QList<QWidget*> customDialogPages;
//     customDialogPages << layoutPage;
    auto* const printDialog = new PrintDialog(printer, QApplication::activeWindow());
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
    auto* const printer = new QPrinter;

    loadConfig(printer);

    auto* const previewDialog = new PrintPreviewDialog(printer, QApplication::activeWindow());

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

        auto errorMessage =
            std::make_unique<Kasten::UserErrorReport>(mByteArrayView,
                                                      message,
                                                      i18nc("@title:window", "Print Byte Array %1", mDocument->title()));
        m_userMessagesHandler->postErrorReport(std::move(errorMessage));
    }
}

void PrintTool::triggerPrintPreview(QPrinter* printer)
{
    std::ignore = doPrint(printer);
}

bool PrintTool::doPrint(QPrinter* printer)
{
    const QString creator = QStringLiteral("Print Plugin for Okteta " OKTETA_VERSION);   // no i18n(), keep space at end as separator
    printer->setCreator(creator);

    FramesPrint::FramesToPaperPrinter framesPrinter;
    const QPageLayout pageLayout = printer->pageLayout();
    const int printerResolution = printer->resolution();
    framesPrinter.setPaperRect(pageLayout.fullRectPixels(printerResolution));
    framesPrinter.setPageRect(pageLayout.paintRectPixels(printerResolution));
    framesPrinter.setPageMargins(pageLayout.marginsPixels(printerResolution));
    printer->setFullPage(true);

    FramesPrint::PrintInfo info;
    const QRect pageRect = framesPrinter.contentsRect();
    const int left = pageRect.left();
    const int width = pageRect.width();

    auto headerFrameRenderer = std::make_unique<FramesPrint::HeaderFooterFrameRenderer>(&info);
    headerFrameRenderer->setTexts(QStringLiteral("%d"),
                                    QStringLiteral("%f"),
                                    i18nc("in the header of the printed page, e.g. Page 2 of 20", "Page %p of %P"));
    headerFrameRenderer->setWidth(width);
    headerFrameRenderer->setPos(pageRect.topLeft());

    auto footerFrameRenderer = std::make_unique<FramesPrint::HeaderFooterFrameRenderer>(&info);
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

    storeConfig(printer);

    auto* const printJob = new PrintJob(&framesPrinter, 0, framesCount - 1, printer);
    const bool success = printJob->exec();

    QApplication::restoreOverrideCursor();

    return success;
}

}

#include "moc_printtool.cpp"
