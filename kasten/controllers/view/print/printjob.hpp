/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTJOB_HPP
#define KASTEN_PRINTJOB_HPP

// KF
// #include <KJob>
// Qt
#include <QObject>

namespace FramesPrint {
class FramesToPaperPrinter;
}

class QPrinter;

namespace Kasten {

class PrintJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    PrintJob(FramesPrint::FramesToPaperPrinter* framesPrinter, int firstPage, int lastPage, QPrinter* printer);

public: // KJob API
//     virtual void start();

public:
    [[nodiscard]]
    bool exec();

private Q_SLOTS:
//     void onFinished();
    void onPagePrinted();

private:
//     PrintThread *mPrintThread;
    FramesPrint::FramesToPaperPrinter* const mFramesPrinter;
    const int mFirstPage;
    const int mLastPage;

    QPrinter* const mPrinter;
};

inline PrintJob::PrintJob(FramesPrint::FramesToPaperPrinter* framesPrinter, int firstPage, int lastPage, QPrinter* printer)
    : mFramesPrinter(framesPrinter)
    , mFirstPage(firstPage)
    , mLastPage(lastPage)
    , mPrinter(printer)
{}

}

#endif
