/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charsetconversionjob.h"

// Okteta core
#include <abstractbytearraymodel.h>
#include <charcodec.h>
#include <character.h>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

static const int CharsetConversionBlockSize = 100000;

bool CharsetConversionJob::exec()
{
    // reset
    mConvertedBytesCount = 0;
    mFailedPerByteCount.clear();

    // check
    if( ! mByteArrayModel || ! mRange.isValid() )
    {
        deleteLater(); // TODO: could be reused on next operation

        return false;
    }

    bool success = true;
    int r = 0;
    Okteta::Address i = mRange.start();
    Okteta::Address blockEnd = mRange.start();
    while( i <= mRange.end() && success )
    {
        blockEnd += CharsetConversionBlockSize;
        if( blockEnd > mRange.end() )
            blockEnd = mRange.end();

        for( ; i<=blockEnd && success; ++i )
        {
            Okteta::Byte convertedByte;
            const Okteta::Byte originalByte = mByteArrayModel->byte(i);
            const Okteta::Character decodedChar = mSourceCharCodec->decode( originalByte );
            bool isConverted = ! decodedChar.isUndefined();
            if( isConverted )
                isConverted = mTargetCharCodec->encode( &convertedByte, decodedChar );

            if( ! isConverted )
            {
                if( ! mSubstitutingMissingChars )
                {
                    success = false;
                    break;
                }
                mFailedPerByteCount[originalByte]++;
                convertedByte = mSubstituteByte;
            }

            // TODO: means if the default byte equals a not convertable original
            // byte that one is not counted. okay, or should be made explicit to user?
            if( originalByte != convertedByte )
                mConvertedBytesCount++;

            mResult[r++] = convertedByte;
        }

        QApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, 100 );
    }

    deleteLater(); // TODO: could be reused on next operation

    return success;
}

}
