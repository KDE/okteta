/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "addresscombobox_p.h"
#include "addresscombobox.h"

// KDE
#include <KLocale>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QAbstractItemView>


/*
Problem: what to do if the format is changed for which the current string is not valid?
Solution: we always convert the string to the new format, so there is never such a situation
*/

namespace Okteta
{

static const QStringList& formatStrings()
{
    static QStringList list;
    if( list.isEmpty() )
    {
//         list.append( i18nc("@item:inlistbox guessing the format of the address by the input",      "Auto") );
        list.append( i18nc( "@item:inlistbox coding of offset in the hexadecimal format", "Hex" ) );
        list.append( i18nc( "@item:inlistbox coding of offset in the decimal format",     "Dec" ) );
    }
    return list;
}

void AddressComboBoxPrivate::init()
{
    Q_Q( AddressComboBox );

    QHBoxLayout* baseLayout = new QHBoxLayout( q );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( 0 );

    mFormatComboBox = new KComboBox( q );
    mFormatComboBox->addItems( formatStrings() );
    q->connect( mFormatComboBox, SIGNAL(activated( int )), SLOT(onFormatChanged( int )) );

    mValueComboBox = new KComboBox( q );
    mValueComboBox->setEditable( true );
    mValueComboBox->setMaxCount( 10 );
    mValueComboBox->setInsertPolicy( QComboBox::NoInsert );
    mValueComboBox->setDuplicatesEnabled( false );
    q->setFocusProxy( mValueComboBox );
    q->connect( mValueComboBox->lineEdit(), SIGNAL(textEdited( const QString& )), SLOT(onValueEdited( const QString& )) );
    QAbstractItemView* formatComboBoxListView = mFormatComboBox->view();
    QObject::connect( formatComboBoxListView, SIGNAL(activated( const QModelIndex& )),
             mValueComboBox, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect( formatComboBoxListView, SIGNAL(pressed( const QModelIndex& )),
             mValueComboBox, SLOT(setFocus()) );
    mValidator = new AddressValidator( mValueComboBox, AddressValidator::HexadecimalCoding );
    mValueComboBox->setValidator( mValidator );
    q->connect( mValueComboBox, SIGNAL(activated( int )), SLOT(onValueActivated( int )) );

    baseLayout->addWidget( mFormatComboBox );
    baseLayout->addWidget( mValueComboBox );
    q->setTabOrder( mFormatComboBox, mValueComboBox );

    onFormatChanged( mFormatComboBox->currentIndex() );
}


void AddressComboBoxPrivate::rememberCurrentAddress()
{
    mValueComboBox->insertItem( -1, mValueComboBox->currentText(), mFormatComboBox->currentIndex() );
}


void AddressComboBoxPrivate::onFormatChanged( int index )
{
    Q_Q( AddressComboBox );

    const QString currentValueText = mValueComboBox->currentText();
    const bool isCurrentValueTextEmpty = currentValueText.isEmpty();
    const Address address = isCurrentValueTextEmpty ? -1 : mValidator->toAddress( currentValueText );

    mValidator->setCodec( static_cast<AddressValidator::Coding>(index) );

    if( ! isCurrentValueTextEmpty )
    {
        const QString convertedValueText = mValidator->toString( address );
        mValueComboBox->setEditText( convertedValueText );
    }

    emit q->formatChanged( index );
}

void AddressComboBoxPrivate::onValueEdited( const QString& value )
{
    Q_Q( AddressComboBox );

    const Address address = mValidator->toAddress( value );

    emit q->addressChanged( address );
}

void AddressComboBoxPrivate::onValueActivated( int index )
{
    Q_Q( AddressComboBox );

    if( index != -1 )
    {
        const int oldFormatIndex = mFormatComboBox->currentIndex();
        const int itemFormatIndex = mValueComboBox->itemData( index ).toInt();

        const bool isOtherFormat = ( oldFormatIndex != itemFormatIndex );
        if( isOtherFormat )
        {
            mFormatComboBox->setCurrentIndex( itemFormatIndex );
            mValidator->setCodec( static_cast<AddressValidator::Coding>(itemFormatIndex) );

        }
        const QString currentValueText = mValueComboBox->currentText();
        const Address address = mValidator->toAddress( currentValueText );

        emit q->addressChanged( address );
        if( isOtherFormat )
            emit q->formatChanged( itemFormatIndex );
    }
}

}
