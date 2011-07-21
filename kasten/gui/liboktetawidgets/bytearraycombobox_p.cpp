/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraycombobox_p.h"
#include "bytearraycombobox.h"

// KDE
#include <KLocale>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QAbstractItemView>


namespace Okteta
{

const QStringList& formatNames()
{
    static QStringList list;
    if( list.isEmpty() )
    {
//         list.append( i18nc("@item:inlistbox guessing the coding of the bytes by the input",      "Auto") );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the hexadecimal format","Hex")  );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the decimal format",    "Dec")      );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the octal format",      "Oct")        );
        list.append( i18nc("@item:inlistbox coding of the bytes as values in the binary format",     "Bin")       );
        list.append( i18nc("@item:inlistbox coding of the bytes as characters with the values",      "Char") );
        list.append( i18nc("@item:inlistbox coding of the bytes as UTF-8 characters with the values","UTF-8") );
    }
    return list;
}


void ByteArrayComboBoxPrivate::init()
{
    Q_Q( ByteArrayComboBox );

    QHBoxLayout *baseLayout = new QHBoxLayout( q );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( 0 );

    mFormatComboBox = new KComboBox( q );
    mFormatComboBox->addItems( formatNames() );
    q->connect( mFormatComboBox, SIGNAL(activated(int)), SLOT(onFormatChanged(int)) );

    mValueComboBox = new KComboBox( q );
    mValueComboBox->setEditable( true );
    mValueComboBox->setMaxCount( 10 );
    mValueComboBox->setInsertPolicy( QComboBox::NoInsert );
    mValueComboBox->setDuplicatesEnabled( false );
    q->setFocusProxy( mValueComboBox );
    q->connect( mValueComboBox->lineEdit(), SIGNAL(textEdited(QString)), SLOT(onValueEdited(QString)) );
    QAbstractItemView* formatComboBoxListView = mFormatComboBox->view();
    QObject::connect( formatComboBoxListView, SIGNAL(activated(QModelIndex)),
             mValueComboBox, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect( formatComboBoxListView, SIGNAL(pressed(QModelIndex)),
             mValueComboBox, SLOT(setFocus()) );
    mValidator = new ByteArrayValidator( mValueComboBox );
    const ByteArrayValidator::Coding coding =
        static_cast<ByteArrayValidator::Coding>( mFormatComboBox->currentIndex() );
    mValidator->setCodec( coding );

    mValueComboBox->setValidator( mValidator );
    q->connect( mValueComboBox, SIGNAL(activated(int)), SLOT(onValueActivated(int)) );

    baseLayout->addWidget( mFormatComboBox );
    baseLayout->addWidget( mValueComboBox, 1 );
    q->setTabOrder( mFormatComboBox, mValueComboBox );
}

void ByteArrayComboBoxPrivate::setByteArray( const QByteArray& byteArray )
{
    mValueComboBox->setEditText( mValidator->toString(byteArray) );
}

void ByteArrayComboBoxPrivate::setCharCodec( const QString& charCodecName )
{
    const bool isChar8Visible = ( mFormatComboBox->currentIndex() == ByteArrayValidator::CharCoding );

    // update the char string if shown
    QByteArray currentByteArray;
    if( isChar8Visible )
    {
        const QString currentChar8String = mValueComboBox->currentText();
        currentByteArray = mValidator->toByteArray( currentChar8String );
    }

    mValidator->setCharCodec( charCodecName );

    if( isChar8Visible )
    {
        const QString char8String = mValidator->toString( currentByteArray );
        mValueComboBox->setEditText( char8String );
    }
}

void ByteArrayComboBoxPrivate::setMaxLength( int maxLength )
{
    const int oldMaxLength = mValidator->maxLength();
    if( oldMaxLength == maxLength )
        return;

    mValidator->setMaxLength( maxLength );

    if( oldMaxLength > maxLength )
    {
        QString currentText = mValueComboBox->currentText();
        int dummyPos;
        mValidator->validate( currentText, dummyPos );
        mValueComboBox->setEditText( currentText );
    }
}

void ByteArrayComboBoxPrivate::setMinLength( int minLength )
{
    const int oldMinLength = mValidator->minLength();
    if( oldMinLength == minLength )
        return;

    mValidator->setMinLength( minLength );

    if( oldMinLength < minLength )
    {
        QString currentText = mValueComboBox->currentText();
        int dummyPos;
        mValidator->validate( currentText, dummyPos );
        mValueComboBox->setEditText( currentText );
    }
}

void ByteArrayComboBoxPrivate::rememberCurrentByteArray()
{
    mValueComboBox->insertItem( -1, mValueComboBox->currentText(), mFormatComboBox->currentIndex() );
}

int ByteArrayComboBoxPrivate::maxLength() const
{
    return mValidator->maxLength();
}

int ByteArrayComboBoxPrivate::minLength() const
{
    return mValidator->minLength();
}


void ByteArrayComboBoxPrivate::onFormatChanged( int index )
{
    Q_Q( ByteArrayComboBox );

    const QString currentValueText = mValueComboBox->currentText();
    const bool isCurrentValueTextEmpty = currentValueText.isEmpty();
    const QByteArray byteArray = isCurrentValueTextEmpty ? QByteArray() : mValidator->toByteArray( currentValueText );

    mValidator->setCodec( static_cast<ByteArrayValidator::Coding>(index) );

    if( ! isCurrentValueTextEmpty )
    {
        const QString convertedValueText = mValidator->toString( byteArray );
        mValueComboBox->setEditText( convertedValueText );
    }

    emit q->formatChanged( index );
}

void ByteArrayComboBoxPrivate::onValueEdited( const QString& value )
{
    Q_Q( ByteArrayComboBox );

    const QByteArray byteArray = mValidator->toByteArray( value );

    emit q->byteArrayChanged( byteArray );
}

void ByteArrayComboBoxPrivate::onValueActivated( int index )
{
    Q_Q( ByteArrayComboBox );

    if( index != -1 )
    {
        const int oldFormatIndex = mFormatComboBox->currentIndex();
        const int itemFormatIndex = mValueComboBox->itemData( index ).toInt();
        const bool isOtherFormat = ( oldFormatIndex != itemFormatIndex );

        if( isOtherFormat )
        {
            mFormatComboBox->setCurrentIndex( itemFormatIndex );
            mValidator->setCodec( static_cast<ByteArrayValidator::Coding>(itemFormatIndex) );

        }
        const QString currentValueText = mValueComboBox->currentText();
        const QByteArray byteArray = mValidator->toByteArray( currentValueText );

        emit q->byteArrayChanged( byteArray );
        if( isOtherFormat )
            emit q->formatChanged( itemFormatIndex );
    }
}

}
