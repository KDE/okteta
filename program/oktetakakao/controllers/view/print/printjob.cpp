/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "printjob.h"

// controller
#include "framestopaperprinter.h"
// #include "printthread.h"
// Qt
#include <QtGui/QApplication>
// #include <QtGui/QFontDatabase>


static const int MaxEventProcessTimeInMS = 100;

#if 0
PrintJob::PrintJob( FramesToPaperPrinter *framesPrinter, int firstPage, int lastPage, QPrinter *printer )
 : mPrintThread( new PrintThread(this,framesPrinter, firstPage, lastPage, printer) )
{
    connect( mPrintThread, SIGNAL(finished()), SLOT(onFinished()) );
}

void PrintJob::start()
{
    if( QFontDatabase::supportsThreadedFontRendering() )
        mPrintThread->start();
}
#endif

bool PrintJob::exec()
{
    connect( mFramesPrinter, SIGNAL(printedPage(int)), SLOT(onPagePrinted()) );

    const bool result = mFramesPrinter->print( mPrinter, mFirstPage, mLastPage );

    deleteLater();

    return result;
}

void PrintJob::onPagePrinted()
{
    QApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, MaxEventProcessTimeInMS );
}

#if 0
void PrintJob::onFinished()
{
    emitResult();
}
#endif
#include "printjob.moc"
