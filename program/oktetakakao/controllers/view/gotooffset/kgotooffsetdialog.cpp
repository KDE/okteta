/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kgotooffsetdialog.h"

// lib
#include <kbytearrayvalidator.h>
// KDE
#include <KComboBox>
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>


static const QStringList &formatStrings()
{
  static QStringList list;
  if( list.isEmpty() )
  {
    list.append( i18nc( "@item:inlistbox coding of offset in the hexadecimal format", "Hexadecimal" ) );
    list.append( i18nc( "@item:inlistbox coding of offset in the decimal format",     "Decimal" )     );
  }
  return list;
}

KGotoOffsetDialog::KGotoOffsetDialog( QWidget *parent )
  : KDialog( parent ),
    mHasView( false )
{
    setCaption( i18nc("@title:window","Go to Offset") );
    setButtons( Ok | Cancel );
    setButtonGuiItem( Ok, KGuiItem(i18nc("@action:button","&Go"), "go-jump",
                      i18nc("@info:tooltip","Go to the Offset"),
                      i18nc("@info:whatsthis","If you press the <interface>Go</interface> "
                            "button, the cursor will be moved in the document to or, "
                            "on your option, by the offset you entered above.")) );
    setDefaultButton( Ok );
    setModal( false );

    QWidget *page = new QWidget( this );
    setMainWidget( page );

    QVBoxLayout *topLayout = new QVBoxLayout( page );
    topLayout->setSpacing( spacingHint() );
    topLayout->setMargin( 0 );

    // search term
    QGroupBox *OffsetBox = new QGroupBox( i18nc("@title:group","Go to"), page );
    topLayout->addWidget( OffsetBox );

    QVBoxLayout *GotoBoxLayout = new QVBoxLayout;
    GotoBoxLayout->setSpacing( spacingHint() );

    QLabel *label = new QLabel( i18nc("@label:listbox","Fo&rmat:"), OffsetBox );
    mSelector = new KComboBox( OffsetBox );
    mSelector->addItems( formatStrings() );
    connect( mSelector, SIGNAL(activated(int)), SLOT(onSelectorChanged(int)) );
    label->setBuddy( mSelector );

    GotoBoxLayout->addWidget( label );
    GotoBoxLayout->addWidget( mSelector );

    label = new QLabel( i18nc("@label:listbox","O&ffset:"), OffsetBox );
    OffsetEdit = new KComboBox( OffsetBox );
    OffsetEdit->setEditable( true );
    OffsetEdit->setMaxCount( 10 );
    OffsetEdit->setInsertPolicy( KComboBox::InsertAtTop );
    connect( OffsetEdit, SIGNAL(editTextChanged(const QString&)), SLOT(onOffsetChanged(const QString&)) );
    mOffsetValidator = new KByteArrayValidator( OffsetEdit, KByteArrayValidator::HexadecimalCoding );
    OffsetEdit->setValidator( mOffsetValidator );
    label->setBuddy( OffsetEdit );
    const QString InputWhatsThis =
        i18nc( "@info:whatsthis","Enter an offset to go to, or select a previous offset from the list." );
    label->setWhatsThis( InputWhatsThis );
    OffsetEdit->setWhatsThis( InputWhatsThis );
    GotoBoxLayout->addWidget( label );
    GotoBoxLayout->addWidget( OffsetEdit );
    OffsetBox->setLayout( GotoBoxLayout );

    // options
    QGroupBox *OptionsBox = new QGroupBox( i18nc("@title:group","Options"), page );
    topLayout->addWidget( OptionsBox );

    QGridLayout *OptionsBoxLayout = new QGridLayout( OptionsBox );
    OptionsBoxLayout->setSpacing( spacingHint() );

    AtCursorCheckBox = new QCheckBox( i18nc("@option:check","From c&ursor"), OptionsBox );
    AtCursorCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go relative from the current cursor location and not absolute.") );
    ExtendSelectionCheckBox = new QCheckBox( i18nc("@option:check","&Extend selection"), OptionsBox );
    ExtendSelectionCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Extend the selection by the cursor move.") );
    BackwardsCheckBox = new QCheckBox( i18nc("@option:check","&Backwards"), OptionsBox );
    BackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go backwards from the end or the current cursor location.") );

    OptionsBoxLayout->addWidget( AtCursorCheckBox, 0, 0 );
    OptionsBoxLayout->addWidget( ExtendSelectionCheckBox, 1, 0 );
    OptionsBoxLayout->addWidget( BackwardsCheckBox, 0, 1 );

    setTabOrder( mSelector, OffsetEdit );
    setTabOrder( OffsetEdit, AtCursorCheckBox );
    setTabOrder( AtCursorCheckBox, BackwardsCheckBox );

    onSelectorChanged( mSelector->currentIndex() );
    enableButtonOk( false );
}

void KGotoOffsetDialog::setRange( int Start, int End )
{
Q_UNUSED( Start )
Q_UNUSED( End )

    // TOOO: handle limits
}

void KGotoOffsetDialog::setHasView( bool hasView )
{
    mHasView = hasView;

    enableButtonOk( mHasView && !OffsetEdit->currentText().isEmpty() );
}


int KGotoOffsetDialog::offset() const
{
    const int HD = mSelector->currentIndex();
    const int Offset = OffsetEdit->currentText().toInt( 0, HD==0?16:10 );
    return Offset;
}

bool KGotoOffsetDialog::isSelectionToExtent() const { return ExtendSelectionCheckBox->isChecked(); }
bool KGotoOffsetDialog::isRelative() const          { return AtCursorCheckBox->isChecked(); }
bool KGotoOffsetDialog::isBackwards() const         { return BackwardsCheckBox->isChecked(); }

void KGotoOffsetDialog::onSelectorChanged( int index )
{
    mOffsetValidator->setCodec( (KByteArrayValidator::Coding)index );
    OffsetEdit->lineEdit()->setText( mOffsetString[ index ] );
}

void KGotoOffsetDialog::onOffsetChanged( const QString &text )
{
    mOffsetString[ mSelector->currentIndex() ] = text;
    enableButtonOk( mHasView && !text.isEmpty() );
}


void KGotoOffsetDialog::showEvent( QShowEvent *showEvent )
{
    KDialog::showEvent(showEvent);
    OffsetEdit->setFocus();
}


KGotoOffsetDialog::~KGotoOffsetDialog() {}
