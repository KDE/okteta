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

#include "createstatisticjob.h"

// Okteta core
#include <abstractbytearraymodel.h>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

static const int StatisticBlockSize = 100000;

int CreateStatisticJob::exec()
{
    // reset
    memset( mByteCount, 0, 256*sizeof(int) );

    const Okteta::Address last = mByteArrayModel ? mSelection.end() : -1;
    Okteta::Address i = mByteArrayModel ? mSelection.start() : 0;
    Okteta::Address blockEnd = i;
    while( i<=last )
    {
        blockEnd += StatisticBlockSize;
        if( blockEnd > last )
            blockEnd = last;

        for( ; i<=blockEnd; ++i )
            ++mByteCount[mByteArrayModel->byte(i)];

        QApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, 100 );
    }

    deleteLater(); // TODO: could be reused on next search

    return ( mByteArrayModel ? mSelection.width() : -1 );
}

}
