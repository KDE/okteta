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

#include "extractstringsjob.h"

// Okteta core
#include <abstractbytearraymodel.h>
#include <charcodec.h>
#include <character.h>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

static const int ExtractStringBlockSize = 100000;

void ExtractStringsJob::exec()
{

    // reset
    mContainedStringList->clear();

    // check
    if( !mByteArrayModel || !mSelection.isValid() )
    {
        deleteLater(); // TODO: could be reused on next operation

        return;
    }

    bool stringStarted = false;
    Okteta::Address stringStart = mSelection.start();
    QString string;

    Okteta::Address i = mSelection.start();
    Okteta::Address blockEnd = mSelection.start();
    while( i<=mSelection.end() )
    {
        blockEnd += ExtractStringBlockSize;
        if( blockEnd > mSelection.end() )
            blockEnd = mSelection.end();

        for( ; i<=blockEnd; ++i )
        {
            const Okteta::Character decodedChar = mCharCodec->decode( mByteArrayModel->byte(i) );
            // TODO: Zeilenumbrüche ausnehmen
            const bool isStringChar = ( !decodedChar.isUndefined() &&
                                        (decodedChar.isLetterOrNumber() || decodedChar.isSpace() || decodedChar.isPunct()) );

            if( isStringChar )
            {
                if( !stringStarted )
                {
                    stringStart = i;
                    stringStarted = true;
                    string.clear();
                }
                string.append( decodedChar );
            }
            else
            {
                if( stringStarted )
                {
                    if( i-stringStart >= mMinLength )
                        mContainedStringList->append( ContainedString(string,stringStart) );
                    stringStarted = false;
                }
            }
        }

        QApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, 100 );
    }

    // last string not ended?
    if( stringStarted )
    {
        if( i-stringStart >= mMinLength )
            mContainedStringList->append( ContainedString(string,stringStart) );
        stringStarted = false;
    }

    deleteLater(); // TODO: could be reused on next operation
}

}
