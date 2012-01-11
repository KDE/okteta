/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef PRINTJOB_H
#define PRINTJOB_H

// KDE
// #include <KJob>
// Qt
#include <QtCore/QObject>

class FramesToPaperPrinter;

class QPrinter;


namespace Kasten2
{

class PrintThread;


class PrintJob : public QObject // not yet: KJob
{
  Q_OBJECT

  public:
    PrintJob( FramesToPaperPrinter *framesPrinter, int firstPage, int lastPage, QPrinter *printer );

  public: // KJob API
//     virtual void start();

  public:
    bool exec();

  protected Q_SLOTS:
//     void onFinished();
    void onPagePrinted();

  protected:
//     PrintThread *mPrintThread;
    FramesToPaperPrinter *mFramesPrinter;
    const int mFirstPage;
    const int mLastPage;

    QPrinter *mPrinter;
};


inline PrintJob::PrintJob( FramesToPaperPrinter *framesPrinter, int firstPage, int lastPage, QPrinter *printer )
 : mFramesPrinter( framesPrinter ), mFirstPage( firstPage ), mLastPage( lastPage ), mPrinter( printer )
{}

}

#endif
