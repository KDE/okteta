/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "insertpatterndialog.h"

// lib
#include <bytearraycombobox.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>


namespace Kasten
{

InsertPatternDialog::InsertPatternDialog( QWidget* parent )
  : KDialog( parent ),
    mHasWriteable( false )
{
    setCaption( i18nc("@title:window","Insert Pattern") );
    setButtons( Ok | Cancel );
    setButtonGuiItem( Ok, KGuiItem(i18nc("@action:button","&Insert"), 0,//"insert",
                      i18nc("@info:tooltip","Insert the pattern"),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Insert</interface> button, "
                            "the pattern you entered above is inserted in the "
                            "byte array at the cursor position.")) );
    setDefaultButton( Ok );
    setModal( false );

    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QVBoxLayout *baseLayout = new QVBoxLayout( page );
    baseLayout->setMargin( 0 );

    // search term
    QGroupBox *insertBox = new QGroupBox( i18nc("@title:group","Insert"), page );
    baseLayout->addWidget( insertBox );

    QVBoxLayout *patternLayout = new QVBoxLayout;

    QLabel *label = new QLabel( i18nc("@label:textbox","Pattern:"), insertBox );
    mPatternEdit = new ByteArrayComboBox( insertBox );
    connect( mPatternEdit, SIGNAL(dataChanged(const QByteArray&)), SLOT(onInputChanged(const QByteArray&)) );
    label->setBuddy( mPatternEdit );
    const QString inputWhatsThis =
        i18nc( "@info:whatsthis", "Enter a pattern to search for, or select a previous pattern from the list." );
    label->setWhatsThis( inputWhatsThis );
    mPatternEdit->setWhatsThis( inputWhatsThis );
    patternLayout->addWidget( label );
    patternLayout->addWidget( mPatternEdit );
    insertBox->setLayout( patternLayout );

    label = new QLabel( i18nc("@label:spinbox number of times to insert the pattern","&Number:"), insertBox );
    mNumberSpinBox = new QSpinBox( insertBox );
    mNumberSpinBox->setRange( 1, INT_MAX );
    mNumberSpinBox->setValue( 1 );
    label->setBuddy( mNumberSpinBox );
    const QString numberWhatsThis =
        i18nc( "@info:whatsthis", "Enter the number of times the pattern should be inserted." );
    label->setWhatsThis( numberWhatsThis );
    mNumberSpinBox->setWhatsThis( numberWhatsThis );

    patternLayout->addWidget( label );
    patternLayout->addWidget( mNumberSpinBox );

    // options
//     QGroupBox *OptionsBox = new QGroupBox( i18n("Options"), page );
//     baseLayout->addWidget( OptionsBox );

//     QGridLayout *OptionsBoxLayout = new QGridLayout( OptionsBox );

    setTabOrder( mPatternEdit, mNumberSpinBox );

    enableButtonOk( false );
}

void InsertPatternDialog::setCharCodec( const QString &codecName )
{
    mPatternEdit->setCharCodec( codecName );
}

void InsertPatternDialog::setHasWriteable( bool hasWriteable )
{
    mHasWriteable = hasWriteable;

    enableButtonOk( mHasWriteable && !mPatternEdit->data().isEmpty() );
}


int InsertPatternDialog::number() const { return mNumberSpinBox->value(); }

QByteArray InsertPatternDialog::pattern() const
{
    return mPatternEdit->data();
}


void InsertPatternDialog::onInputChanged( const QByteArray &data )
{
    enableButtonOk( mHasWriteable && !data.isEmpty() );
}


void InsertPatternDialog::showEvent( QShowEvent *event )
{
    KDialog::showEvent( event );
    mPatternEdit->setFocus();
}


InsertPatternDialog::~InsertPatternDialog() {}

}
