/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTTOOL_HPP
#define KASTEN_PRINTTOOL_HPP

// Qt
#include <QObject>
#include <QPageLayout>
#include <QPageSize>

namespace Okteta {
class AbstractByteArrayModel;
}

class QPrinter;

namespace Kasten {

class ByteArrayView;
class ByteArrayDocument;
class AbstractModel;
class AbstractUserMessagesHandler;

/**
   This tool cares for the printing of the byte array to a series of papers.
   This is done by creating a series of print commands, which may also
   be in form of a pdf document. We don't care for this, it's done
   by Qt and the print dialog.

   The content is printed into a series of frames. By default there are
   a header frame, the content frame and the footer frame on each page.


   -> Header printer, footer printer, content printer
   -> called by a pageprinter which knows about the layout
   -> pageprinter controls the page settings and informs the embedded printer
   -> frameprinter return number of frames they need for their content
   -> endless frames vs. ending frames
   -> vars for each frame: see KWrite
   ->

 */
class PrintTool : public QObject
{
    Q_OBJECT

private:
    static constexpr char ConfigGroupId[] = "PrintTool";

    static constexpr char PageLayoutUnitConfigKey[] = "PageLayoutUnit";
    static constexpr char PageMarginLeftConfigKey[] = "PageMarginLeft";
    static constexpr char PageMarginRightConfigKey[] = "PageMarginRight";
    static constexpr char PageMarginTopConfigKey[] = "PageMarginTop";
    static constexpr char PageMarginBottomConfigKey[] = "PageMarginBottom";
    static constexpr char PageSizeIdConfigKey[] = "PageSizeId";
    static constexpr char PageSizeUnitConfigKey[] = "PageSizeUnit";
    static constexpr char PageWidthConfigKey[] = "PageWidth";
    static constexpr char PageHeightConfigKey[] = "PageHeight";
    static constexpr char PageOrientationConfigKey[] = "PageOrientation";

    static constexpr QPageLayout::Unit DefaultPageLayoutUnit = QPageLayout::Millimeter;
    static constexpr qreal DefaultPageMarginLeft = 20; // mm
    static constexpr qreal DefaultPageMarginRight = 20; // mm
    static constexpr qreal DefaultPageMarginTop = 20; // mm
    static constexpr qreal DefaultPageMarginBottom = 20; // mm
    static constexpr QPageLayout::Orientation DefaultPageOrientation = QPageLayout::Portrait;
    static constexpr QPageSize::PageSizeId DefaultPageSizeId = QPageSize::A4;
    static constexpr QPageSize::Unit DefaultPageSizeUnit = QPageSize::Millimeter;

public:
    explicit PrintTool(AbstractUserMessagesHandler* userMessagesHandler);
    ~PrintTool() override;

public:
    void setTargetModel(AbstractModel* model);

public Q_SLOTS:
    void print();
    void printPreview();

Q_SIGNALS:
    void viewChanged(bool hasView);

private Q_SLOTS:
    void triggerPrint(QPrinter* printer);
    void triggerPrintPreview(QPrinter* printer);

private:
    [[nodiscard]]
    bool doPrint(QPrinter* printer);
    void loadConfig(QPrinter* printer) const;
    void storeConfig(const QPrinter* printer);

private:
    ByteArrayDocument* mDocument = nullptr;

    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
