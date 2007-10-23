/***************************************************************************
                          kgotooffsetdialog.cpp  -  description
                             -------------------
    begin                : Thu Nov 23 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kgotooffsetdialog.h"

// 
#include <kbytearrayvalidator.h>
// KDE
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
// #include <kstdguiitem.h>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>
// #include <QValidator>


static const QStringList &formatStrings()
{
  static QStringList list;
  if( list.isEmpty() )
  {
    list.append( i18n( "Hexadecimal" ) );
    list.append( i18n( "Decimal" ) );
  }
  return list;
}

KGotoOffsetDialog::KGotoOffsetDialog( QWidget *parent )
  : KDialog( parent )
{
    setCaption( i18n("Go to Offset") );
    setButtons( Ok | Cancel );
    setButtonGuiItem( Ok, KGuiItem(i18n("&Go"), "go",
                      i18n("Go to the Offset"),
                      i18n("<qt>If you press the <b>Go</b> button, the cursor will be moved "
                           "in the document to or, on your option, by the offset you entered above.</qt>")));
    setDefaultButton( Ok );
    setModal( false );

    QWidget *Page = new QWidget( this );
    setMainWidget( Page );

    QVBoxLayout *topLayout = new QVBoxLayout( Page );
    topLayout->setSpacing( spacingHint() );
    topLayout->setMargin( 0 );

    // search term
    QGroupBox *OffsetBox = new QGroupBox( i18n("Go to"), Page );
    topLayout->addWidget( OffsetBox );

    QVBoxLayout *GotoBoxLayout = new QVBoxLayout;
    GotoBoxLayout->setSpacing( spacingHint() );

    QLabel *label = new QLabel( i18n("Fo&rmat:"), OffsetBox );
    mSelector = new QComboBox( OffsetBox );
    mSelector->addItems( formatStrings() );
    connect( mSelector, SIGNAL(activated(int)), SLOT(onSelectorChanged(int)) );
    label->setBuddy( mSelector );

    GotoBoxLayout->addWidget( label );
    GotoBoxLayout->addWidget( mSelector );

    label = new QLabel( i18n("O&ffset:"), OffsetBox );
    OffsetEdit = new QComboBox( OffsetBox );
    OffsetEdit->setEditable( true );
    OffsetEdit->setMaxCount( 10 );
    OffsetEdit->setInsertPolicy( QComboBox::InsertAtTop );
    connect( OffsetEdit, SIGNAL(textChanged(const QString&)), SLOT(onOffsetChanged(const QString&)) );
    mOffsetValidator = new KByteArrayValidator( OffsetEdit, KHECore::HexadecimalCoding );
    OffsetEdit->setValidator( mOffsetValidator );
    label->setBuddy( OffsetEdit );
    const QString InputWhatsThis =
        i18n("Enter an offset to go to, or select a previous offset from the list.");
    label->setWhatsThis( InputWhatsThis );
    OffsetEdit->setWhatsThis( InputWhatsThis );
    GotoBoxLayout->addWidget( label );
    GotoBoxLayout->addWidget( OffsetEdit );
    OffsetBox->setLayout( GotoBoxLayout );

    // options
    QGroupBox *OptionsBox = new QGroupBox( i18n("Options"), Page );
    topLayout->addWidget( OptionsBox );

    QGridLayout *OptionsBoxLayout = new QGridLayout( OptionsBox );
    OptionsBoxLayout->setSpacing( spacingHint() );

    AtCursorCheckBox = new QCheckBox( i18n("From c&ursor"), OptionsBox );
    OptionsBoxLayout->addWidget( AtCursorCheckBox, 0, 0 );
    AtCursorCheckBox->setWhatsThis( i18n("Go relative from the current cursor location and not absolute.") );
    BackwardsCheckBox = new QCheckBox( i18n("&Backwards"), OptionsBox );
    OptionsBoxLayout->addWidget( BackwardsCheckBox, 0, 1 );
    BackwardsCheckBox->setWhatsThis(i18n("Go backwards from the end or the current cursor location.") );

    setTabOrder( mSelector, OffsetEdit );
    setTabOrder( OffsetEdit, AtCursorCheckBox );
    setTabOrder( AtCursorCheckBox, BackwardsCheckBox );

    onSelectorChanged( mSelector->currentIndex() );
    enableButtonOk( false );
}

void KGotoOffsetDialog::setRange( int Start, int End )
{
    Start= End; // TOOO: handle limits
}

int KGotoOffsetDialog::offset() const
{
    const int HD = mSelector->currentIndex();
    const int Offset = OffsetEdit->currentText().toInt( 0, HD==0?16:10 );
    return BackwardsCheckBox->isChecked() ? -Offset : Offset;
}

bool KGotoOffsetDialog::isRelative() const { return AtCursorCheckBox->isChecked(); }


void KGotoOffsetDialog::onSelectorChanged( int index )
{
    mOffsetValidator->setCoding( index );
    OffsetEdit->lineEdit()->setText( mOffsetString[ index ] );
}

void KGotoOffsetDialog::onOffsetChanged( const QString &text )
{
    mOffsetString[ mSelector->currentIndex() ] = text;
    enableButtonOk( !text.isEmpty() );
}


void KGotoOffsetDialog::showEvent( QShowEvent *e )
{
    KDialog::showEvent(e);
    OffsetEdit->setFocus();
}


KGotoOffsetDialog::~KGotoOffsetDialog()
{
}


#include "kgotooffsetdialog.moc"
