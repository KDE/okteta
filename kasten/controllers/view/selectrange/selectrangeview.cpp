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

#include "selectrangeview.h"

// tool
#include "selectrangetool.h"
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
#include <QtGui/QAbstractItemView>
#include <QtGui/QLayout>


namespace Kasten
{

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


SelectRangeView::SelectRangeView( SelectRangeTool* tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    QHBoxLayout* baseLayout = new QHBoxLayout( this );
    baseLayout->setMargin( 0 );

    // start offset
    QGridLayout* startOffsetLayout = new QGridLayout();
    startOffsetLayout->setMargin( 0 );
    startOffsetLayout->setColumnStretch( 0, 0 );
    startOffsetLayout->setColumnStretch( 1, 0 );

    QLabel* label = new QLabel( i18nc("@label:listbox","Format:"), this );
    mStartFormatSelector = new KComboBox( this );
    mStartFormatSelector->addItems( formatStrings() );
    connect( mStartFormatSelector, SIGNAL(activated(int)), SLOT(onStartSelectorChanged(int)) );
    label->setBuddy( mStartFormatSelector );

    startOffsetLayout->addWidget( label, 0, 0, Qt::AlignRight);
    startOffsetLayout->addWidget( mStartFormatSelector, 0, 1);

    label = new QLabel( i18nc("@label:listbox","Start offset:"), this );
    mStartEdit = new KComboBox( this );
    mStartEdit->setEditable( true );
    mStartEdit->setMaxCount( 10 );
    mStartEdit->setInsertPolicy( KComboBox::InsertAtTop );
    connect( mStartEdit, SIGNAL(editTextChanged(const QString&)), SLOT(onStartChanged(const QString&)) );
    QAbstractItemView* formatComboBoxListView = mStartFormatSelector->view();
    connect( formatComboBoxListView, SIGNAL(activated( const QModelIndex& )),
             mStartEdit, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    connect( formatComboBoxListView, SIGNAL(pressed( const QModelIndex& )),
             mStartEdit, SLOT(setFocus()) );
    mStartValidator = new KByteArrayValidator( mStartEdit, KByteArrayValidator::HexadecimalCoding );
    mStartEdit->setValidator( mStartValidator );
    label->setBuddy( mStartEdit );
    const QString startInputWhatsThis =
        i18nc( "@info:whatsthis","Enter an offset to go to, or select a previous offset from the list." );
    label->setWhatsThis( startInputWhatsThis );
    mStartEdit->setWhatsThis( startInputWhatsThis );

    startOffsetLayout->addWidget( label, 1, 0, Qt::AlignRight);
    startOffsetLayout->addWidget( mStartEdit, 1, 1);
    setFocusProxy( mStartEdit ); // TODO: see how KDialog does it, e.g. see if there is already a focuswidget as child

    baseLayout->addLayout( startOffsetLayout );

    // end offset
    QGridLayout* endOffsetLayout = new QGridLayout();
    endOffsetLayout->setMargin( 0 );
    endOffsetLayout->setColumnStretch( 0, 0 );
    endOffsetLayout->setColumnStretch( 1, 0 );

    label = new QLabel( i18nc("@label:listbox","Format:"), this );
    mEndFormatSelector = new KComboBox( this );
    mEndFormatSelector->addItems( formatStrings() );
    connect( mEndFormatSelector, SIGNAL(activated(int)), SLOT(onEndSelectorChanged(int)) );
    label->setBuddy( mEndFormatSelector );

    endOffsetLayout->addWidget( label, 0, 0, Qt::AlignRight);
    endOffsetLayout->addWidget( mEndFormatSelector, 0, 1);

    label = new QLabel( i18nc("@label:listbox","End offset:"), this );
    mEndEdit = new KComboBox( this );
    mEndEdit->setEditable( true );
    mEndEdit->setMaxCount( 10 );
    mEndEdit->setInsertPolicy( KComboBox::InsertAtTop );
    connect( mEndEdit, SIGNAL(editTextChanged(const QString&)), SLOT(onEndChanged(const QString&)) );
    formatComboBoxListView = mEndFormatSelector->view();
    connect( formatComboBoxListView, SIGNAL(activated( const QModelIndex& )),
             mEndEdit, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    connect( formatComboBoxListView, SIGNAL(pressed( const QModelIndex& )),
             mEndEdit, SLOT(setFocus()) );
    mEndValidator = new KByteArrayValidator( mEndEdit, KByteArrayValidator::HexadecimalCoding );
    mEndEdit->setValidator( mEndValidator );
    label->setBuddy( mEndEdit );
    const QString endInputWhatsThis =
        i18nc( "@info:whatsthis","Enter an offset to go to, or select a previous offset from the list." );
    label->setWhatsThis( endInputWhatsThis );
    mEndEdit->setWhatsThis( endInputWhatsThis );

    endOffsetLayout->addWidget( label, 1, 0, Qt::AlignRight);
    endOffsetLayout->addWidget( mEndEdit, 1, 1);

    baseLayout->addLayout( endOffsetLayout );

    // options
    QVBoxLayout* optionsLayout = new QVBoxLayout();
    optionsLayout->setMargin( 0 );

    mRelativeCheckBox = new QCheckBox( i18nc("@option:check","End relative"), this );
    mRelativeCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Extend the selection by the cursor move.") );
    connect( mRelativeCheckBox, SIGNAL(toggled( bool )), mTool, SLOT(setIsEndRelative( bool )) );
    mBackwardsCheckBox = new QCheckBox( i18nc("@option:check","&Backwards"), this );
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go backwards from the end or the current cursor location.") );
    connect( mBackwardsCheckBox, SIGNAL(toggled( bool )), mTool, SLOT(setIsEndBackwards( bool )) );

    connect( mRelativeCheckBox, SIGNAL(toggled( bool )), mBackwardsCheckBox, SLOT(setEnabled( bool )) );
    mBackwardsCheckBox->setEnabled( false );

    optionsLayout->addWidget( mRelativeCheckBox );
    optionsLayout->addWidget( mBackwardsCheckBox );

    baseLayout->addLayout( optionsLayout );

    // Select button
    const KGuiItem selectGuiItem( i18nc("@action:button","&Select"), 0,
                      i18nc("@info:tooltip","Select the range."),
                      i18nc("@info:whatsthis","If you press the <interface>Select</interface> "
                            "button, the cursor will be moved in the document to or, "
                            "on your option, by the offset you entered above.") );
    mSelectButton = new KPushButton( selectGuiItem, this );
    connect( mSelectButton, SIGNAL(clicked(bool)), SLOT(onSelectButtonClicked()) );
    baseLayout->addWidget( mSelectButton );
    baseLayout->setAlignment( mSelectButton, Qt::AlignTop );

    baseLayout->addStretch();

    setTabOrder( mStartFormatSelector, mStartEdit );
    setTabOrder( mEndFormatSelector, mEndEdit );
    setTabOrder( mEndEdit, mRelativeCheckBox );
    setTabOrder( mRelativeCheckBox, mBackwardsCheckBox );
    setTabOrder( mBackwardsCheckBox, mSelectButton );

    connect( mTool, SIGNAL(isApplyableChanged( bool )), SLOT( onApplyableChanged( bool )) );

    onStartSelectorChanged( mStartFormatSelector->currentIndex() );
    onEndSelectorChanged( mEndFormatSelector->currentIndex() );
    onApplyableChanged( mTool->isApplyable() );
}


void SelectRangeView::onApplyableChanged( bool isApplyable )
{
    // TODO: set error tooltip, like offset out of range or no document
    // TODO: set color flag to offset input
    mSelectButton->setEnabled( isApplyable );
}

void SelectRangeView::onStartSelectorChanged( int index )
{
    mStartValidator->setCodec( static_cast<KByteArrayValidator::Coding>(index) );
    mStartEdit->lineEdit()->setText( mStartString[ index ] );
}

void SelectRangeView::onEndSelectorChanged( int index )
{
    mEndValidator->setCodec( static_cast<KByteArrayValidator::Coding>(index) );
    mEndEdit->lineEdit()->setText( mEndString[ index ] );
}

void SelectRangeView::onStartChanged( const QString& text )
{
    const int formatIndex = mStartFormatSelector->currentIndex();
    mStartString[formatIndex] = text;

    const int isHexadecimal = ( formatIndex == 0 );
    const int base = isHexadecimal ? 16 : 10;
    const int offset = mStartEdit->currentText().toInt( 0, base );

    mTool->setTargetStart( offset );
}

void SelectRangeView::onEndChanged( const QString& text )
{
    const int formatIndex = mEndFormatSelector->currentIndex();
    mEndString[formatIndex] = text;

    const int isHexadecimal = ( formatIndex == 0 );
    const int base = isHexadecimal ? 16 : 10;
    const int offset = mEndEdit->currentText().toInt( 0, base );

    mTool->setTargetEnd( offset );
}

void SelectRangeView::onSelectButtonClicked()
{
    mTool->select();
//     emit toolUsed();
}


SelectRangeView::~SelectRangeView() {}

}
