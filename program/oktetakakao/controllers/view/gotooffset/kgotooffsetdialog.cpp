/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// tool
#include "gotooffsettool.h"
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
#include <QtGui/QAbstractItemView>
#include <QtGui/QLayout>


static const QStringList& formatStrings()
{
    static QStringList list;
    if( list.isEmpty() )
    {
        list.append( i18nc( "@item:inlistbox coding of offset in the hexadecimal format", "Hexadecimal" ) );
        list.append( i18nc( "@item:inlistbox coding of offset in the decimal format",     "Decimal" )     );
    }
    return list;
}

KGotoOffsetDialog::KGotoOffsetDialog( GotoOffsetTool* tool, QWidget* parent )
  : KDialog( parent ),
    mTool( tool )
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

    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QVBoxLayout* topLayout = new QVBoxLayout( page );
    topLayout->setMargin( 0 );

    // search term
    QGroupBox* offsetBox = new QGroupBox( i18nc("@title:group","Go To"), page );
    topLayout->addWidget( offsetBox );

    QVBoxLayout* gotoBoxLayout = new QVBoxLayout;

    QLabel* label = new QLabel( i18nc("@label:listbox","Fo&rmat:"), offsetBox );
    mSelector = new KComboBox( offsetBox );
    mSelector->addItems( formatStrings() );
    connect( mSelector, SIGNAL(activated(int)), SLOT(onSelectorChanged(int)) );
    label->setBuddy( mSelector );

    gotoBoxLayout->addWidget( label );
    gotoBoxLayout->addWidget( mSelector );

    label = new QLabel( i18nc("@label:listbox","O&ffset:"), offsetBox );
    mOffsetEdit = new KComboBox( offsetBox );
    mOffsetEdit->setEditable( true );
    mOffsetEdit->setMaxCount( 10 );
    mOffsetEdit->setInsertPolicy( KComboBox::InsertAtTop );
    connect( mOffsetEdit, SIGNAL(editTextChanged(const QString&)), SLOT(onOffsetChanged(const QString&)) );
    QAbstractItemView* formatComboBoxListView = mSelector->view();
    connect( formatComboBoxListView, SIGNAL(activated( const QModelIndex& )),
             mOffsetEdit, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    connect( formatComboBoxListView, SIGNAL(pressed( const QModelIndex& )),
             mOffsetEdit, SLOT(setFocus()) );
    mOffsetValidator = new KByteArrayValidator( mOffsetEdit, KByteArrayValidator::HexadecimalCoding );
    mOffsetEdit->setValidator( mOffsetValidator );
    label->setBuddy( mOffsetEdit );
    const QString inputWhatsThis =
        i18nc( "@info:whatsthis","Enter an offset to go to, or select a previous offset from the list." );
    label->setWhatsThis( inputWhatsThis );
    mOffsetEdit->setWhatsThis( inputWhatsThis );
    gotoBoxLayout->addWidget( label );
    gotoBoxLayout->addWidget( mOffsetEdit );
    offsetBox->setLayout( gotoBoxLayout );

    // options
    QGroupBox* optionsBox = new QGroupBox( i18nc("@title:group","Options"), page );
    topLayout->addWidget( optionsBox );

    QGridLayout* optionsBoxLayout = new QGridLayout( optionsBox );

    mAtCursorCheckBox = new QCheckBox( i18nc("@option:check","From c&ursor"), optionsBox );
    mAtCursorCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go relative from the current cursor location and not absolute.") );
    connect( mAtCursorCheckBox, SIGNAL(clicked( bool )),
             mTool, SLOT(setIsRelative( bool )) );

    mExtendSelectionCheckBox = new QCheckBox( i18nc("@option:check","&Extend selection"), optionsBox );
    mExtendSelectionCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Extend the selection by the cursor move.") );
    connect( mExtendSelectionCheckBox, SIGNAL(clicked( bool )),
             mTool, SLOT(setIsSelectionToExtent( bool )) );

    mBackwardsCheckBox = new QCheckBox( i18nc("@option:check","&Backwards"), optionsBox );
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go backwards from the end or the current cursor location.") );
    connect( mBackwardsCheckBox, SIGNAL(clicked( bool )),
             mTool, SLOT(setIsBackwards( bool )) );

    optionsBoxLayout->addWidget( mAtCursorCheckBox, 0, 0 );
    optionsBoxLayout->addWidget( mExtendSelectionCheckBox, 1, 0 );
    optionsBoxLayout->addWidget( mBackwardsCheckBox, 0, 1 );

    setTabOrder( mSelector, mOffsetEdit );
    setTabOrder( mOffsetEdit, mAtCursorCheckBox );
    setTabOrder( mAtCursorCheckBox, mBackwardsCheckBox );

    connect( mTool, SIGNAL(isApplyableChanged( bool )), SLOT(onToolIsApplyableChanged( bool )) );

    onSelectorChanged( mSelector->currentIndex() );
    onToolIsApplyableChanged( mTool->isApplyable() );
}

void KGotoOffsetDialog::setOffset( int offset )
{
    const int isHexadecimal = ( mSelector->currentIndex() == 0 );
    const int base = isHexadecimal ? 16 : 10;
    mOffsetEdit->lineEdit()->setText( QString::number(offset,base) );
}

void KGotoOffsetDialog::setRange( int firstOffset, int lastOffset )
{
Q_UNUSED( firstOffset )
Q_UNUSED( lastOffset )

    // TOOO: handle limits
}

void KGotoOffsetDialog::onToolIsApplyableChanged( bool isApplyable )
{
    enableButtonOk( isApplyable && !mOffsetEdit->currentText().isEmpty() );
}


int KGotoOffsetDialog::offset() const
{
    const int isHexadecimal = ( mSelector->currentIndex() == 0 );
    const int base = isHexadecimal ? 16 : 10;
    const int offset = mOffsetEdit->currentText().toInt( 0, base );
    return offset;
}

void KGotoOffsetDialog::onSelectorChanged( int index )
{
    mOffsetValidator->setCodec( static_cast<KByteArrayValidator::Coding>(index) );
    mOffsetEdit->lineEdit()->setText( mOffsetString[ index ] );
}

void KGotoOffsetDialog::onOffsetChanged( const QString& text )
{
    mOffsetString[ mSelector->currentIndex() ] = text;
    mTool->setTargetOffset( offset() );
    enableButtonOk( mTool->isApplyable() && !text.isEmpty() );
}


void KGotoOffsetDialog::slotButtonClicked( int button )
{
    if( button != KDialog::Ok )
        KDialog::slotButtonClicked( button );
    else
    {
        hide();

        mTool->gotoOffset();
    }
}


void KGotoOffsetDialog::showEvent( QShowEvent* showEvent )
{
    KDialog::showEvent( showEvent );

//     const int startOffset = mByteArrayDisplay->startOffset();
//         mGotoOffsetDialog->setRange( startOffset, startOffset+mByteArray->size()-1 );
//         mGotoOffsetDialog->setHasView( true );

    setOffset( mTool->currentOffset() );
    mOffsetEdit->setFocus();
}


KGotoOffsetDialog::~KGotoOffsetDialog() {}
