/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "stringsextracttool.h"

// lib
#include <kbytearraydocument.h>
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kbytearraymodel.h>


static const int DefaultMinLength = 3;

ContainedString::ContainedString( const QString &string, int offset )
 : mString( string ), mOffset( offset )
{}

void ContainedString::move( int offset )
{
    mOffset += offset;
}


StringsExtractTool::StringsExtractTool()
 : mDocument( 0 ), mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ), mMinLength( DefaultMinLength )
{
}

QList<ContainedString> StringsExtractTool::containedStringList() const { return mContainedStringList; }

void StringsExtractTool::setDocument( KByteArrayDocument *document )
{
    mDocument = document;
}

void StringsExtractTool::setCharCodec( const QString &codecName )
{
    if( codecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::KCharCodec::createCodec( codecName );
}


// TODO: extract only from section
// TODO: use KWordBufferService
void StringsExtractTool::extract()
{
    mContainedStringList.clear();
    if( !mDocument )
        return;

    KHECore::KAbstractByteArrayModel *byteArrayModel = mDocument->content();
    int start = 0; 
    int end = byteArrayModel->size();

    bool wordStarted = false;
    int wordStart = start;
    QString word;
    int i;
    for( i = start; i<end; ++i )
    {
        const KHECore::KChar decodedChar = mCharCodec->decode( byteArrayModel->datum(i) );
        const bool isWordChar = ( !decodedChar.isUndefined() && decodedChar.isLetterOrNumber() );

        if( isWordChar )
        {
            if( !wordStarted )
            {
                wordStart = i;
                wordStarted = true;
                word.clear();
            }
            word.append( decodedChar );
        }
        else
        {
            if( wordStarted )
            {
                if( i-wordStart >= mMinLength )
                    mContainedStringList.append( ContainedString(word,wordStart) );
                wordStarted = false;
            }
        }
    }
    // last word not ended?
    if( wordStarted )
    {
        if( i-wordStart >= mMinLength )
            mContainedStringList.append( ContainedString(word,wordStart) );
        wordStarted = false;
    }

    emit stringsChanged();
}


StringsExtractTool::~StringsExtractTool()
{
    delete mCharCodec;
}
