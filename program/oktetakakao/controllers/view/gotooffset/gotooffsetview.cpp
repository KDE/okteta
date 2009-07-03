/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "gotooffsetview.h"

// tool
#include "gotooffsettool.h"
// lib
#include <kbytearrayvalidator.h>
// KDE
#include <KComboBox>
#include <KPushButton>
#include <KLineEdit>
#include <KGuiItem>
#include <KLocale>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>


static const int MinimumStringLength = 1;

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


GotoOffsetView::GotoOffsetView( GotoOffsetTool* tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    QHBoxLayout* baseLayout = new QHBoxLayout( this );
    baseLayout->setMargin( 0 );

    // offset
    QGridLayout* offsetLayout = new QGridLayout();
    offsetLayout->setMargin( 0 );
    offsetLayout->setColumnStretch( 0, 0 );
    offsetLayout->setColumnStretch( 1, 0 );

    QLabel* label = new QLabel( i18nc("@label:listbox","Fo&rmat:"), this );
    mFormatSelector = new KComboBox( this );
    mFormatSelector->addItems( formatStrings() );
    connect( mFormatSelector, SIGNAL(activated(int)), SLOT(onSelectorChanged(int)) );
    label->setBuddy( mFormatSelector );

    offsetLayout->addWidget( label, 0, 0, Qt::AlignRight);
    offsetLayout->addWidget( mFormatSelector, 0, 1);

    label = new QLabel( i18nc("@label:listbox","O&ffset:"), this );
    mOffsetEdit = new KComboBox( this );
    mOffsetEdit->setEditable( true );
    mOffsetEdit->setMaxCount( 10 );
    mOffsetEdit->setInsertPolicy( KComboBox::InsertAtTop );
    connect( mOffsetEdit, SIGNAL(editTextChanged(const QString&)), SLOT(onOffsetChanged(const QString&)) );
    mOffsetValidator = new KByteArrayValidator( mOffsetEdit, KByteArrayValidator::HexadecimalCoding );
    mOffsetEdit->setValidator( mOffsetValidator );
    label->setBuddy( mOffsetEdit );
    const QString inputWhatsThis =
        i18nc( "@info:whatsthis","Enter an offset to go to, or select a previous offset from the list." );
    label->setWhatsThis( inputWhatsThis );
    mOffsetEdit->setWhatsThis( inputWhatsThis );

    offsetLayout->addWidget( label, 1, 0, Qt::AlignRight);
    offsetLayout->addWidget( mOffsetEdit, 1, 1);

    baseLayout->addLayout( offsetLayout );

    // options
    QVBoxLayout* optionsLayout = new QVBoxLayout();
    optionsLayout->setMargin( 0 );

    mAtCursorCheckBox = new QCheckBox( i18nc("@option:check","From c&ursor"), this );
    mAtCursorCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go relative from the current cursor location and not absolute.") );
    connect( mAtCursorCheckBox, SIGNAL(toggled( bool )), mTool, SLOT(setIsRelative( bool )) );
    mExtendSelectionCheckBox = new QCheckBox( i18nc("@option:check","&Extend selection"), this );
    mExtendSelectionCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Extend the selection by the cursor move.") );
    connect( mExtendSelectionCheckBox, SIGNAL(toggled( bool )), mTool, SLOT(setIsSelectionToExtent( bool )) );
    mBackwardsCheckBox = new QCheckBox( i18nc("@option:check","&Backwards"), this );
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go backwards from the end or the current cursor location.") );
    connect( mBackwardsCheckBox, SIGNAL(toggled( bool )), mTool, SLOT(setIsBackwards( bool )) );

    QHBoxLayout* upperOptionsLayout = new QHBoxLayout();
    upperOptionsLayout->setMargin( 0 );
    upperOptionsLayout->addWidget( mAtCursorCheckBox );
    upperOptionsLayout->addWidget( mBackwardsCheckBox );

    optionsLayout->addLayout( upperOptionsLayout );
    optionsLayout->addWidget( mExtendSelectionCheckBox );
    optionsLayout->addStretch();

    baseLayout->addLayout( optionsLayout );

    // Goto button
    const KGuiItem gotoGuiItem( i18nc("@action:button","&Go"), "go-jump",
                      i18nc("@info:tooltip","Go to the Offset"),
                      i18nc("@info:whatsthis","If you press the <interface>Go</interface> "
                            "button, the cursor will be moved in the document to or, "
                            "on your option, by the offset you entered above.") );
    mGotoButton = new KPushButton( gotoGuiItem, this );
    connect( mGotoButton, SIGNAL(clicked(bool)), SLOT(onGotoButtonClicked()) );
    baseLayout->addWidget( mGotoButton );
    baseLayout->setAlignment( mGotoButton, Qt::AlignTop );

    baseLayout->addStretch();

    setTabOrder( mFormatSelector, mOffsetEdit );
    setTabOrder( mOffsetEdit, mAtCursorCheckBox );
    setTabOrder( mAtCursorCheckBox, mBackwardsCheckBox );
    setTabOrder( mBackwardsCheckBox, mExtendSelectionCheckBox );
    setTabOrder( mExtendSelectionCheckBox, mGotoButton );

    connect( mTool, SIGNAL(isApplyableChanged( bool )), SLOT( onApplyableChanged( bool )) );

    onSelectorChanged( mFormatSelector->currentIndex() );
    onApplyableChanged( mTool->isApplyable() );
}


void GotoOffsetView::onApplyableChanged( bool isApplyable )
{
    // TODO: set error tooltip, like offset out of range or no document
    // TODO: set color flag to offset input
    mGotoButton->setEnabled( isApplyable );
}

void GotoOffsetView::onSelectorChanged( int index )
{
    mOffsetValidator->setCodec( static_cast<KByteArrayValidator::Coding>(index) );
    mOffsetEdit->lineEdit()->setText( mOffsetString[ index ] );
}

void GotoOffsetView::onOffsetChanged( const QString& text )
{
    const int formatIndex = mFormatSelector->currentIndex();
    mOffsetString[formatIndex] = text;

    const int isHexadecimal = ( formatIndex == 0 );
    const int base = isHexadecimal ? 16 : 10;
    const int offset = mOffsetEdit->currentText().toInt( 0, base );

    mTool->setTargetOffset( offset );
}

void GotoOffsetView::onGotoButtonClicked()
{
    mTool->gotoOffset();
//     emit toolUsed();
}


GotoOffsetView::~GotoOffsetView() {}
