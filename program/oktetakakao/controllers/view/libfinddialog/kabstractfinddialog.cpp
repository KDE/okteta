/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include <kbytearraylineedit.h>
// Okteta core
#include <khe.h>
// KDE
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
// #include <kstandardguiitem.h>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>


KAbstractFindDialog::KAbstractFindDialog( QWidget *parent )
  : KDialog( parent )
{
    setButtons( Ok | Cancel );
    setDefaultButton( Ok );
    enableButtonOk( false );

    QWidget *Page = new QWidget( this );
    setMainWidget( Page );

    QVBoxLayout *PageLayout = new QVBoxLayout( Page );
    PageLayout->setSpacing( spacingHint() );
    PageLayout->setMargin( 0 );

    // find term
    QGroupBox *FindBox = new QGroupBox( i18n("Find"), Page );
    PageLayout->addWidget( FindBox );

    QVBoxLayout *FindBoxLayout = new QVBoxLayout;
    FindBoxLayout->setSpacing( spacingHint() );

    QLabel *BytesLabel = new QLabel( i18n("Bytes to F&ind:"), FindBox );
    SearchDataEdit = new KByteArrayLineEdit( FindBox );
    connect( SearchDataEdit, SIGNAL(dataChanged(const QByteArray&)), SLOT(onSearchDataChanged(const QByteArray&)) );
    connect( SearchDataEdit, SIGNAL(formatChanged(int)), SLOT(onSearchDataFormatChanged(int)) );
    BytesLabel->setBuddy( SearchDataEdit );
    const QString InputWhatsThis =
        i18n("Enter a pattern to search for, or select a previous pattern from the list.");
    BytesLabel->setWhatsThis( InputWhatsThis );
    SearchDataEdit->setWhatsThis( InputWhatsThis );

    FindBoxLayout->addWidget( BytesLabel );
    FindBoxLayout->addWidget( SearchDataEdit );
    FindBox->setLayout( FindBoxLayout );

    // options
    QGroupBox *OptionsBox = new QGroupBox( i18n("Options"), Page );
    PageLayout->addWidget( OptionsBox );

    QGridLayout *OptionsBoxLayout = new QGridLayout( OptionsBox );
    OptionsBoxLayout->setSpacing( spacingHint() );

    CaseSensitiveCheckBox = new QCheckBox( i18n("C&ase sensitive"),OptionsBox);
    CaseSensitiveCheckBox->setWhatsThis( i18n("Perform a case sensitive search: "
            "entering the pattern 'Joe' will not match 'joe' or 'JOE', only 'Joe'.") );
    WholeWordsCheckBox = new QCheckBox( i18n("&Whole words only"),OptionsBox );
    WholeWordsCheckBox->setWhatsThis( i18n("Require word boundaries in both ends of a match to succeed.") );
    AtCursorCheckBox = new QCheckBox( i18n("From c&ursor"), OptionsBox );
    AtCursorCheckBox->setWhatsThis( i18n("Start searching at the current cursor location rather than at the top.") );

    BackwardsCheckBox = new QCheckBox( i18n("&Backwards"), OptionsBox );
    BackwardsCheckBox->setWhatsThis(i18n("Replace backwards.") );
    SelectedCheckBox = new QCheckBox( i18n("&Selected bytes"), OptionsBox );
    SelectedCheckBox->setWhatsThis( i18n("Only search within the current selection.") );

    OptionsBoxLayout->addWidget( CaseSensitiveCheckBox, 0, 0 );
    OptionsBoxLayout->addWidget( WholeWordsCheckBox, 1, 0 );
    OptionsBoxLayout->addWidget( AtCursorCheckBox, 2, 0 );
    OptionsBoxLayout->addWidget( BackwardsCheckBox, 0, 1 );
    OptionsBoxLayout->addWidget( SelectedCheckBox, 1, 1 );

//     setTabOrder( SearchDataFormatComboBox, SearchDataEdit );

//     setTabOrder( LastTabWidget, CaseSensitiveCheckBox );
    setTabOrder( CaseSensitiveCheckBox, WholeWordsCheckBox );
    setTabOrder( WholeWordsCheckBox, AtCursorCheckBox );
    setTabOrder( AtCursorCheckBox, BackwardsCheckBox );
    setTabOrder( BackwardsCheckBox, SelectedCheckBox );

    onSearchDataFormatChanged( SearchDataEdit->format() );
}

void KAbstractFindDialog::setOperationBox( QGroupBox *OperationBox )
{
    QVBoxLayout *PageLayout = static_cast<QVBoxLayout *>( mainWidget()->layout() );
    PageLayout->insertWidget( 1, OperationBox );
}

void KAbstractFindDialog::setExtraOption( QCheckBox* OptionCheckBox )
{
    if( OptionCheckBox )
    {
        QGridLayout *OptionsBoxLayout = static_cast<QGridLayout *>( BackwardsCheckBox->parentWidget()->layout() );
        OptionsBoxLayout->addWidget( OptionCheckBox, 2, 1 );
        setTabOrder( SelectedCheckBox, OptionCheckBox );
    }
}


bool KAbstractFindDialog::fromCursor()            const { return AtCursorCheckBox->isChecked(); }
bool KAbstractFindDialog::inSelection()           const { return SelectedCheckBox->isChecked(); }
bool KAbstractFindDialog::ignoreCase()            const { return !CaseSensitiveCheckBox->isChecked(); }
KFindDirection KAbstractFindDialog::direction() const { return BackwardsCheckBox->isChecked() ? FindBackward : FindForward; }

QByteArray KAbstractFindDialog::data()  const
{
    return SearchDataEdit->data();
}

void KAbstractFindDialog::setDirection( KFindDirection Direction )
{
    BackwardsCheckBox->setChecked( Direction == FindBackward );
}

void KAbstractFindDialog::setInSelection( bool InSelection )
{
    SelectedCheckBox->setChecked( InSelection );
}

void KAbstractFindDialog::setCharCode( const QString &CodeName )
{
    SearchDataEdit->setCharCode( CodeName );
}

void KAbstractFindDialog::onSearchDataFormatChanged( int index )
{
    CaseSensitiveCheckBox->setEnabled( index == KHECore::CharCoding );
    WholeWordsCheckBox->setEnabled( index == KHECore::CharCoding );
}

void KAbstractFindDialog::onSearchDataChanged( const QByteArray &Data )
{
    enableButtonOk( !Data.isEmpty() );
}


void KAbstractFindDialog::showEvent( QShowEvent *e )
{
  KDialog::showEvent(e);
  SearchDataEdit->setFocus();
}


KAbstractFindDialog::~KAbstractFindDialog()
{
}
