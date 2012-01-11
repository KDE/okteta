/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "kabstractfinddialog.h"

// lib
#include <bytearraycombobox.h>
// Okteta core
#include <oktetacore.h>
// KDE
#include <KGlobal>
#include <KLocale>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>


namespace Kasten2
{

KAbstractFindDialog::KAbstractFindDialog( QWidget* parent )
  : KDialog( parent )
{
    setButtons( Ok | Cancel );
    setDefaultButton( Ok );

    // TODO: setting ok button to disabled as before done gets overwritten to true
    // if setting the button gui with an inline KGuiItem in the subclass,
    // which has no parameter for enabled and defaults to true
}

void KAbstractFindDialog::setupFindBox()
{
    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QVBoxLayout *pageLayout = new QVBoxLayout( page );
    pageLayout->setMargin( 0 );

    // find term
    QGroupBox *findBox = new QGroupBox( i18nc("@title:window","Find"), page );
    pageLayout->addWidget( findBox );

    QVBoxLayout *findBoxLayout = new QVBoxLayout;

    SearchDataEdit = new Okteta::ByteArrayComboBox( findBox );
    connect( SearchDataEdit, SIGNAL(byteArrayChanged(QByteArray)),
             SLOT(onSearchDataChanged(QByteArray)) );
    connect( SearchDataEdit, SIGNAL(formatChanged(int)),
             SLOT(onSearchDataFormatChanged(int)) );
    const QString toolTip =
        i18nc("@info:tooltip",
              "Enter the bytes to search for, or select bytes previously searched for from the list.");
    SearchDataEdit->setToolTip( toolTip );

    findBoxLayout->addWidget( SearchDataEdit );
    findBox->setLayout( findBoxLayout );
}

void KAbstractFindDialog::setupOperationBox( QGroupBox *operationBox )
{
    QVBoxLayout *pageLayout = static_cast<QVBoxLayout *>( mainWidget()->layout() );

    // operation box
    if( operationBox )
        pageLayout->addWidget( operationBox );
}

void KAbstractFindDialog::setupCheckBoxes( QCheckBox *optionCheckBox )
{
    QWidget* page = mainWidget();
    QVBoxLayout *pageLayout = static_cast<QVBoxLayout *>( mainWidget()->layout() );

    // options
    QGroupBox *optionsBox = new QGroupBox( i18nc("@title:group","Options"), page );
    pageLayout->addWidget( optionsBox );

    QGridLayout *optionsBoxLayout = new QGridLayout( optionsBox );

    CaseSensitiveCheckBox = new QCheckBox( i18nc("@option:check","C&ase sensitive"),optionsBox);
    CaseSensitiveCheckBox->setWhatsThis( i18nc("@info:whatsthis","Perform a case sensitive search: "
            "entering the pattern 'Joe' will not match 'joe' or 'JOE', only 'Joe'.") );
    WholeWordsCheckBox = new QCheckBox( i18nc("@option:check","&Whole words only"),optionsBox );
    WholeWordsCheckBox->setWhatsThis( i18nc("@info:whatsthis","Require word boundaries in both ends of a match to succeed.") );
    AtCursorCheckBox = new QCheckBox( i18nc("@option:check","From c&ursor"), optionsBox );
    AtCursorCheckBox->setWhatsThis( i18nc("@info:whatsthis","Start searching at the current cursor location rather than at the top.") );

    BackwardsCheckBox = new QCheckBox( i18nc("@option:check","&Backwards"), optionsBox );
    BackwardsCheckBox->setWhatsThis(i18nc("@info:whatsthis","Replace backwards.") );
    SelectedCheckBox = new QCheckBox( i18nc("@option:check","&Selected bytes"), optionsBox );
    SelectedCheckBox->setWhatsThis( i18nc("@info:whatsthis","Only search within the current selection.") );

    optionsBoxLayout->addWidget( CaseSensitiveCheckBox, 0, 0 );
    optionsBoxLayout->addWidget( WholeWordsCheckBox, 1, 0 );
    optionsBoxLayout->addWidget( AtCursorCheckBox, 2, 0 );
    optionsBoxLayout->addWidget( BackwardsCheckBox, 0, 1 );
    optionsBoxLayout->addWidget( SelectedCheckBox, 1, 1 );
    if( optionCheckBox )
        optionsBoxLayout->addWidget( optionCheckBox, 2, 1 );

    setTabOrder( CaseSensitiveCheckBox, WholeWordsCheckBox );
    setTabOrder( WholeWordsCheckBox, AtCursorCheckBox );
    setTabOrder( AtCursorCheckBox, BackwardsCheckBox );
    setTabOrder( BackwardsCheckBox, SelectedCheckBox );
//     if( optionCheckBox )
//         setTabOrder( SelectedCheckBox, optionCheckBox );

    onSearchDataFormatChanged( SearchDataEdit->format() );
}

bool KAbstractFindDialog::fromCursor()            const { return AtCursorCheckBox->isChecked(); }
bool KAbstractFindDialog::inSelection()           const { return SelectedCheckBox->isChecked(); }
KFindDirection KAbstractFindDialog::direction() const
{
    return BackwardsCheckBox->isChecked() ? FindBackward : FindForward;
}
Qt::CaseSensitivity KAbstractFindDialog::caseSensitivity() const
{
    return ( SearchDataEdit->format() == Okteta::ByteArrayComboBox::CharCoding )
           && ! CaseSensitiveCheckBox->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive;
}

QByteArray KAbstractFindDialog::data()  const
{
    return SearchDataEdit->byteArray();
}

void KAbstractFindDialog::setDirection( KFindDirection Direction )
{
    BackwardsCheckBox->setChecked( Direction == FindBackward );
}

void KAbstractFindDialog::setInSelection( bool InSelection )
{
    SelectedCheckBox->setChecked( InSelection );
}

void KAbstractFindDialog::setCharCodec( const QString &codecName )
{
    SearchDataEdit->setCharCodec( codecName );
}

void KAbstractFindDialog::rememberCurrentSettings()
{
    SearchDataEdit->rememberCurrentByteArray();
}

void KAbstractFindDialog::onSearchDataFormatChanged( int index )
{
    const bool isCharCoding = ( index == Okteta::ByteArrayComboBox::CharCoding );
    CaseSensitiveCheckBox->setEnabled( isCharCoding );
    WholeWordsCheckBox->setEnabled( false );//isCharCoding ); TODO: not implemented!
}

void KAbstractFindDialog::onSearchDataChanged( const QByteArray &data )
{
    enableButtonOk( !data.isEmpty() );
}


void KAbstractFindDialog::showEvent( QShowEvent *showEvent )
{
  KDialog::showEvent(showEvent);
  SearchDataEdit->setFocus();
}


KAbstractFindDialog::~KAbstractFindDialog() {}

}
