/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FRAMESPRINT_FRAMESTOPAPERPRINTER_HPP
#define FRAMESPRINT_FRAMESTOPAPERPRINTER_HPP

// Qt
#include <QObject>
#include <QRect>
// Std
#include <memory>
#include <vector>

class QPrinter;

namespace FramesPrint {

class AbstractFrameRenderer;

class FramesToPaperPrinter : public QObject
{
    Q_OBJECT

public:
    FramesToPaperPrinter();
    ~FramesToPaperPrinter() override;

public:
    void setPaperRect(QRect paperRect);
    void setPageRect(QRect pageRect);
    void setPageMargins(QMargins pageMargins);

    void addFrameRenderer(std::unique_ptr<AbstractFrameRenderer>&& frameRenderer);

    [[nodiscard]]
    bool print(QPrinter* printer, int firstPageIndex, int lastPageIndex);

public:
    [[nodiscard]]
    QRect pageRect() const;
    [[nodiscard]]
    QMargins pageMargins() const;
    [[nodiscard]]
    QRect contentsRect() const;

Q_SIGNALS:
    void printedPage(int pageIndex);

private:
    std::vector<std::unique_ptr<AbstractFrameRenderer>> mFrameRendererList;
    QRect mPaperRect;
    QRect mPageRect;
    QMargins mPageMargins;
};

}

#endif
