/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "searchjob.h"

// filter
#include <abstractbytearrayfilter.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <character.h>
#include <charcodec.h>
// Qt
#include <QtGui/QApplication>


namespace Kasten
{

static const int MaxEventProcessTimeInMS = 100;
static const int IgnoreCaseSearchedByteCountSignalLimit = 10000;


SearchJob::SearchJob( const Okteta::AbstractByteArrayModel* model,
                      const QByteArray& searchData, int startIndex, bool findForward, bool ignoreCase, const QString& charCodecName )
  : mByteArrayModel( model ),
    mSearchData( searchData ),
    mStartIndex( startIndex),
    mFindForward( findForward ),
    mIgnoreCase( ignoreCase ),
    mCharCodec( Okteta::CharCodec::createCodec(charCodecName) )
{
    if( mIgnoreCase )
    {
        // create variant of search data with all letters lowercase
        const int patternLength = mSearchData.size();
        char* pattern = mSearchData.data();
        for( int i = 0; i<patternLength; ++i )
        {
            const Okteta::Character decodedChar = mCharCodec->decode( pattern[i] );

            if( decodedChar.isUndefined() )
                continue;

            mCharCodec->encode( &pattern[i], decodedChar.toLower() );
        }
    }
}


int SearchJob::indexOfIgnoreCase()
{
    int result = -1;

    const char* const pattern = mSearchData.constData();
    const int patternLength = mSearchData.size();
    const int lastFrom = mByteArrayModel->size() - patternLength;

    int nextSignalByteCount = mStartIndex + IgnoreCaseSearchedByteCountSignalLimit;

    for( int i=mStartIndex; i<=lastFrom ; ++i )
    {
        int c = 0;
        for( ; c<patternLength; ++c )
        {
            char byte = mByteArrayModel->datum( i+c );

            // turn to lowercase if possible
            // TODO: optimize, like caching and not reencoding chars without a lower letter
            const Okteta::Character decodedChar = mCharCodec->decode( byte );
            if( ! decodedChar.isUndefined() )
                mCharCodec->encode( &byte, decodedChar.toLower() );

            if( byte != pattern[c]  )
                break;
        }

        if( nextSignalByteCount <= i )
        {
            nextSignalByteCount += IgnoreCaseSearchedByteCountSignalLimit;
            onBytesSearched();//( i-fromOffset+1 );
        }

        if( c == patternLength )
        {
            result = i;
            break;
        }
    }

    return result;
}

int SearchJob::lastIndexOfIgnoreCase()
{
    int result = -1;

    const char* const pattern = mSearchData.constData();
    const int patternLength = mSearchData.size();
    const int lastFrom = mByteArrayModel->size() - patternLength;

    const int fromOffset =
        ( mStartIndex < 0 ) ?        lastFrom + 1 + mStartIndex :
        ( mStartIndex > lastFrom ) ? lastFrom :
        /* else */                   mStartIndex;

    int nextSignalByteCount = fromOffset - IgnoreCaseSearchedByteCountSignalLimit;

    for( int i=fromOffset; i>=0 ; --i )
    {
        int c = 0;
        for( ; c<patternLength; ++c )
        {
            char byte = mByteArrayModel->datum( i+c );

            // turn to lowercase if possible
            // TODO: optimize, like caching and not reencoding chars without a lower letter
            const Okteta::Character decodedChar = mCharCodec->decode( byte );
            if( ! decodedChar.isUndefined() )
                mCharCodec->encode( &byte, decodedChar.toLower() );

            if( byte != pattern[c]  )
                break;
        }

        if( nextSignalByteCount >= i )
        {
            nextSignalByteCount -= IgnoreCaseSearchedByteCountSignalLimit;
            onBytesSearched();//( i-fromOffset+1 );
        }

        if( c == patternLength )
        {
            result = i;
            break;
        }
    }

    return result;
}

int SearchJob::exec()
{
    //TODO: what kind of signal could a filter send?
    connect( mByteArrayModel, SIGNAL(searchedBytes(int)), SLOT(onBytesSearched()) );

    const int result = mFindForward ?
        ( mIgnoreCase ?
            indexOfIgnoreCase() :
            mByteArrayModel->indexOf( mSearchData, mStartIndex ) ) :
        ( mIgnoreCase ?
            lastIndexOfIgnoreCase() :
            mByteArrayModel->lastIndexOf( mSearchData, mStartIndex-mSearchData.size()+1 ) );

    deleteLater(); // TODO: could be reused on next search

    return result;
}

void SearchJob::onBytesSearched()
{
    QApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, MaxEventProcessTimeInMS );
}


SearchJob::~SearchJob()
{
    delete mCharCodec;
}

}
