/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printjob.hpp"

// controller
#include "framestopaperprinter.hpp"
// #include "printthread.hpp"
// Qt
#include <QCoreApplication>
// #include <QFontDatabase>

namespace Kasten {

#if 0
PrintJob::PrintJob(FramesToPaperPrinter* framesPrinter, int firstPage, int lastPage, QPrinter* printer)
    : mPrintThread(new PrintThread(this, framesPrinter, firstPage, lastPage, printer))
{
    connect(mPrintThread, SIGNAL(finished()), SLOT(onFinished()));
}

void PrintJob::start()
{
    if (QFontDatabase::supportsThreadedFontRendering()) {
        mPrintThread->start();
    }
}
#endif

bool PrintJob::exec()
{
    connect(mFramesPrinter, &FramesToPaperPrinter::printedPage, this, &PrintJob::onPagePrinted);

    const bool result = mFramesPrinter->print(mPrinter, mFirstPage, mLastPage);

    deleteLater();

    return result;
}

void PrintJob::onPagePrinted()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
}

#if 0
void PrintJob::onFinished()
{
    emitResult();
}
#endif

}
