/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewconfigcontroller.h"

// lib
#include "bytesperlinedialog.h"
#include "bytespergroupdialog.h"
#include <bytearrayview.h>
// Okteta core
#include <charcodec.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KActionCollection>
#include <KSelectAction>
#include <KToggleAction>


namespace Kasten2
{

ViewConfigController::ViewConfigController( KXMLGUIClient* guiClient )
 : mByteArrayView( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    // value valueCoding
    mCodingAction = actionCollection->add<KSelectAction>( QLatin1String("view_valuecoding") );
    mCodingAction->setText( i18nc("@title:menu","&Value Coding") );
    QStringList list;
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format",
                       "&Hexadecimal") );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",
                       "&Decimal") );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",
                       "&Octal") );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",
                       "&Binary") );
    mCodingAction->setItems( list );
    connect( mCodingAction, SIGNAL(triggered(int)), SLOT(setValueCoding(int)) );

    // char valueCoding
    mEncodingAction = actionCollection->add<KSelectAction>( QLatin1String("view_charencoding") );
    mEncodingAction->setText( i18nc("@title:menu","&Char Coding") );
    mEncodingAction->setItems( Okteta::CharCodec::codecNames() );
    connect( mEncodingAction, SIGNAL(triggered(int)), SLOT(setCharCoding(int)) );

    mShowsNonprintingAction = actionCollection->add<KToggleAction>( QLatin1String("view_showsnonprinting") );
    mShowsNonprintingAction->setText( i18nc("@option:check","Show &Non-printing Chars") );
    connect( mShowsNonprintingAction, SIGNAL(triggered(bool)), SLOT(setShowsNonprinting(bool)) );

    // bytes per line
    mSetBytesPerLineAction = actionCollection->addAction( QLatin1String("view_bytesperline"),
                                                          this, SLOT(setBytesPerLine()) );
    mSetBytesPerLineAction->setText( i18nc("@action:inmenu","Set Bytes per Line...") );

    // byte groups size
    mSetBytesPerGroupAction = actionCollection->addAction( QLatin1String("view_bytespergroup"),
                                                           this, SLOT(setBytesPerGroup()) );
    mSetBytesPerGroupAction->setText( i18nc("@action:inmenu","Set Bytes per Group...") );

    // resize style
    mResizeStyleAction = actionCollection->add<KSelectAction>( QLatin1String("resizestyle") );
    mResizeStyleAction->setText( i18nc("@title:menu","&Dynamic Layout") );
    list.clear();
    list.append( i18nc("@item:inmenu  The layout will not change on size changes.",
                       "&Off") );
    list.append( i18nc("@item:inmenu  The layout will adapt to the size, but only with complete groups of bytes.",
                       "&Wrap Only Complete Byte Groups") );
    list.append( i18nc("@item:inmenu  The layout will adapt to the size and fit in as much bytes per line as possible.",
                       "&On") );
    mResizeStyleAction->setItems( list );
    connect( mResizeStyleAction, SIGNAL(triggered(int)), SLOT(setLayoutStyle(int)) );

    mShowOffsetColumnAction = actionCollection->add<KToggleAction>( QLatin1String("view_lineoffset") );
    mShowOffsetColumnAction->setText( i18nc("@option:check","Show &Line Offset") );
    mShowOffsetColumnAction->setShortcut( Qt::Key_F11 );
    connect( mShowOffsetColumnAction, SIGNAL(triggered(bool)), SLOT(toggleOffsetColumn(bool)) );

    // show buffer columns
    mToggleColumnsAction = actionCollection->add<KSelectAction>( QLatin1String("togglecolumns") );
    mToggleColumnsAction->setText( i18nc("@title:menu","&Show Values or Chars") );
    list.clear();
    list.append( i18nc("@item:inmenu","&Values") );
    list.append( i18nc("@item:inmenu","&Chars") );
    list.append( i18nc("@item:inmenu","Values && Chars") );
    mToggleColumnsAction->setItems( list );
    connect( mToggleColumnsAction, SIGNAL(triggered(int)), SLOT(toggleValueCharColumns(int)) );

    setTargetModel( 0 );
}

void ViewConfigController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    const bool hasView = ( mByteArrayView != 0 );
    if( hasView )
    {
        onOffsetColumnVisibleChanged( mByteArrayView->offsetColumnVisible() );
        onShowsNonprintingChanged( mByteArrayView->showsNonprinting() );
        onValueCodingChanged( mByteArrayView->valueCoding() );
        onCharCodecChanged( mByteArrayView->charCodingName() );
        onLayoutStyleChanged( mByteArrayView->layoutStyle() );
        onVisibleByteArrayCodingsChanged( mByteArrayView->visibleByteArrayCodings() );

        connect( mByteArrayView, SIGNAL(offsetColumnVisibleChanged(bool)),
                 SLOT(onOffsetColumnVisibleChanged(bool)) );
        connect( mByteArrayView, SIGNAL(showsNonprintingChanged(bool)),
                 SLOT(onShowsNonprintingChanged(bool)) );
        connect( mByteArrayView, SIGNAL(valueCodingChanged(int)), SLOT(onValueCodingChanged(int)) );
        connect( mByteArrayView, SIGNAL(charCodecChanged(QString)),
            SLOT(onCharCodecChanged(QString)) );
        connect( mByteArrayView, SIGNAL(layoutStyleChanged(int)), SLOT(onLayoutStyleChanged(int)) );
        connect( mByteArrayView, SIGNAL(visibleByteArrayCodingsChanged(int)),
                 SLOT(onVisibleByteArrayCodingsChanged(int)) );
    }

    mCodingAction->setEnabled( hasView );
    mEncodingAction->setEnabled( hasView );
    mShowsNonprintingAction->setEnabled( hasView );
    mSetBytesPerLineAction->setEnabled( hasView );
    mSetBytesPerGroupAction->setEnabled( hasView );
    mResizeStyleAction->setEnabled( hasView );
    mShowOffsetColumnAction->setEnabled( hasView );
    mToggleColumnsAction->setEnabled( hasView );
}


void ViewConfigController::setValueCoding( int valueCoding )
{
    mByteArrayView->setValueCoding( valueCoding );
}

void ViewConfigController::setShowsNonprinting( bool on )
{
    mByteArrayView->setShowsNonprinting( on );
}

void ViewConfigController::toggleOffsetColumn( bool on )
{
    mByteArrayView->toggleOffsetColumn( on );
}

void ViewConfigController::setBytesPerLine()
{
    BytesPerLineDialog dialog;
    dialog.setBytesPerLine( mByteArrayView->noOfBytesPerLine() );
    if( dialog.exec() )
    {
        mByteArrayView->setNoOfBytesPerLine( dialog.bytesPerLine() );

        // TODO: change should be signalled and the action listen to that
        mResizeStyleAction->setCurrentItem( mByteArrayView->layoutStyle() );
    }
}

void ViewConfigController::setBytesPerGroup()
{
    BytesPerGroupDialog dialog;
    dialog.setGroupedBytesCount( mByteArrayView->noOfGroupedBytes() );
    if( dialog.exec() )
        mByteArrayView->setNoOfGroupedBytes( dialog.groupedBytesCount() );
}

void ViewConfigController::setLayoutStyle( int layoutStyle )
{
    mByteArrayView->setLayoutStyle( layoutStyle );
}

void ViewConfigController::setCharCoding( int charCoding )
{
    mByteArrayView->setCharCoding( Okteta::CharCodec::codecNames()[charCoding] );
}

void ViewConfigController::toggleValueCharColumns( int visibleColumns )
{
    mByteArrayView->setVisibleByteArrayCodings( visibleColumns+1 );
}

void ViewConfigController::onOffsetColumnVisibleChanged( bool offsetColumnVisible )
{
    mShowOffsetColumnAction->setChecked( offsetColumnVisible );
}

void ViewConfigController::onShowsNonprintingChanged( bool showsNonprinting )
{
    mShowsNonprintingAction->setChecked( showsNonprinting );
}

void ViewConfigController::onValueCodingChanged( int valueCoding )
{
    mCodingAction->setCurrentItem( valueCoding );
}

void ViewConfigController::onCharCodecChanged( const QString& charCodecName )
{
    const int charCodingIndex = Okteta::CharCodec::codecNames().indexOf( charCodecName );

    mEncodingAction->setCurrentItem( charCodingIndex );
}

void ViewConfigController::onLayoutStyleChanged( int layoutStyle )
{
    mResizeStyleAction->setCurrentItem( layoutStyle );
}

void ViewConfigController::onVisibleByteArrayCodingsChanged( int visibleByteArrayCodings )
{
    mToggleColumnsAction->setCurrentItem( visibleByteArrayCodings-1 );
}

}
