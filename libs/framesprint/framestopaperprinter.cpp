/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "framestopaperprinter.hpp"

// lib
#include "abstractframerenderer.hpp"
// Qt
#include <QPainter>
#include <QPrinter>
// Std
#include <utility>

FramesToPaperPrinter::FramesToPaperPrinter() = default;

FramesToPaperPrinter::~FramesToPaperPrinter() = default;

QRect FramesToPaperPrinter::pageRect() const { return mPageRect; }
QMargins FramesToPaperPrinter::pageMargins() const { return mPageMargins; }
QRect FramesToPaperPrinter::contentsRect() const { return mPaperRect - mPageMargins; }

void FramesToPaperPrinter::setPaperRect(QRect paperRect) { mPaperRect = paperRect; }
void FramesToPaperPrinter::setPageRect(QRect pageRect) { mPageRect = pageRect; }
void FramesToPaperPrinter::setPageMargins(QMargins pageMargins) { mPageMargins = pageMargins; }

void FramesToPaperPrinter::addFrameRenderer(std::unique_ptr<AbstractFrameRenderer>&& frameRenderer)
{
    mFrameRendererList.emplace_back(std::move(frameRenderer));
}

bool FramesToPaperPrinter::print(QPrinter* printer, int firstPageIndex, int lastPageIndex)
{
    bool success = true;

    for (const auto& frameRenderer : mFrameRendererList) {
        frameRenderer->prepare();
    }

    QPainter painter(printer);
    int pageIndex = firstPageIndex;
    while (true) {
        for (const auto& frameRenderer : mFrameRendererList) {
            const int x = frameRenderer->x();
            const int y = frameRenderer->y();
            painter.translate(x, y);
            frameRenderer->renderFrame(&painter, pageIndex);
            painter.translate(-x, -y);
        }

        Q_EMIT printedPage(pageIndex);
        if (pageIndex < lastPageIndex) {
            printer->newPage();
            ++pageIndex;
        } else {
            break;
        }
    }

    return success;
}

#include "moc_framestopaperprinter.cpp"
