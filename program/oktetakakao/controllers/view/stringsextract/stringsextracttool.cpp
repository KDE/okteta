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
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kbytearraymodel.h>


static const int DefaultMinLength = 3;

StringsExtractTool::StringsExtractTool()
 : mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ), mMinLength( DefaultMinLength ),
   mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

bool StringsExtractTool::isApplyable() const
{
    return ( mByteArrayModel != 0 && mByteArrayView->hasSelectedData() );
}

// TODO: add model with offset and string
// doubleclick moves cursor to offset
// filter für Suche, inkl. Regulärausdrücke
// groß/kleinschreibung
// voll strings, auch mit Leerzeichen
void StringsExtractTool::setView( KAbstractView *view )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView )
    {
        connect( mByteArrayView,  SIGNAL(charCodecChanged( const QString & )),
                 SLOT(setCharCodec( const QString &)) );
        connect( mByteArrayView,  SIGNAL(selectionChanged( bool )),
                 SIGNAL(isApplyableChanged( bool )) );
        connect( mByteArrayModel,  SIGNAL(modificationChanged( bool )),
                 SIGNAL(isApplyableChanged( bool )) );

        setCharCodec( mByteArrayView->encodingName() );
    }

    // TODO: if there is no view, there is nothing to extract.
    // or this could be the view where we got the strings from and it did not change
    emit isApplyableChanged( isApplyable() );
}

void StringsExtractTool::setMinLength( int minLength )
{
    mMinLength = minLength;
}

void StringsExtractTool::setCharCodec( const QString &codecName )
{
    if( codecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::KCharCodec::createCodec( codecName );
}

void StringsExtractTool::selectString( int stringId )
{
    const ContainedString &containedString = mContainedStringList.at(stringId);
    const int offset = containedString.offset();
    const int length = containedString.string().length();
    mByteArrayView->setSelection( offset, offset+length-1 );
    mByteArrayView->setFocus();
}

// TODO: use KWordBufferService
void StringsExtractTool::extractStrings()
{
    mContainedStringList.clear();
    if( !mByteArrayModel )
        return;

    const KHE::KSection selection = mByteArrayView->selection();
    if( !selection.isValid() )
        return;

    bool stringStarted = false;
    int stringStart = selection.start();
    QString string;
    int i;
    for( i = selection.start(); i<=selection.end(); ++i )
    {
        const KHECore::KChar decodedChar = mCharCodec->decode( mByteArrayModel->datum(i) );
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
                    mContainedStringList.append( ContainedString(string,stringStart) );
                stringStarted = false;
            }
        }
    }
    // last string not ended?
    if( stringStarted )
    {
        if( i-stringStart >= mMinLength )
            mContainedStringList.append( ContainedString(string,stringStart) );
        stringStarted = false;
    }

    emit stringsUpdated();
}


StringsExtractTool::~StringsExtractTool()
{
    delete mCharCodec;
}
