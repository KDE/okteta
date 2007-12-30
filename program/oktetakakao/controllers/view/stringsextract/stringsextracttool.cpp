/***************************************************************************
                          stringsextracttool.cpp  -  description
                             -------------------
    begin                : Wed Dec 26 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "stringsextracttool.h"

//
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
