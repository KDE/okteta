/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
#include <addresscombobox.h>
#include <addressvalidator.h>
// KF5
#include <QPushButton>
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QLabel>
#include <QLayout>


namespace Kasten2
{

GotoOffsetView::GotoOffsetView( GotoOffsetTool* tool, QWidget* parent )
  : AbstractToolWidget( parent ),
    mTool( tool )
{
    QHBoxLayout* baseLayout = new QHBoxLayout( this );
    baseLayout->setMargin( 0 );

    // offset
    QHBoxLayout* offsetLayout = new QHBoxLayout();
    offsetLayout->setMargin( 0 );

    QLabel* label = new QLabel( i18nc("@label:listbox","O&ffset:"), this );
    mAddressEdit = new Okteta::AddressComboBox( this );
    connect( mAddressEdit, &Okteta::AddressComboBox::addressChanged,
             mTool, &GotoOffsetTool::setTargetOffset );
    connect( mAddressEdit, &Okteta::AddressComboBox::formatChanged,
             this, &GotoOffsetView::onFormatChanged );
    connect( mAddressEdit, &Okteta::AddressComboBox::addressTypeChanged,
             this, &GotoOffsetView::onAddressTypeChanged );
    label->setBuddy( mAddressEdit );
    const QString inputWhatsThis =
        i18nc( "@info:whatsthis","Enter an offset to go to, or select a previous offset from the list." );
    label->setWhatsThis( inputWhatsThis );
    mAddressEdit->setWhatsThis( inputWhatsThis );

    offsetLayout->addWidget( label );
    offsetLayout->addWidget( mAddressEdit, 1 );

    baseLayout->addLayout( offsetLayout, 1 );
    baseLayout->setAlignment( offsetLayout, Qt::AlignTop );

    setFocusProxy( mAddressEdit ); // TODO: see how KDialog does it, e.g. see if there is already a focuswidget as child

    // options
    QVBoxLayout* optionsLayout = new QVBoxLayout();
    optionsLayout->setMargin( 0 );

    mAtCursorCheckBox = new QCheckBox( i18nc("@option:check","From c&ursor"), this );
    mAtCursorCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go relative from the current cursor location and not absolute.") );
    connect( mAtCursorCheckBox, &QCheckBox::toggled,
             mTool, &GotoOffsetTool::setIsRelative );
    mExtendSelectionCheckBox = new QCheckBox( i18nc("@option:check","&Extend selection"), this );
    mExtendSelectionCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Extend the selection by the cursor move.") );
    connect( mExtendSelectionCheckBox, &QCheckBox::toggled,
             mTool, &GotoOffsetTool::setIsSelectionToExtent );
    mBackwardsCheckBox = new QCheckBox( i18nc("@option:check","&Backwards"), this );
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis","Go backwards from the end or the current cursor location.") );
    connect( mBackwardsCheckBox, &QCheckBox::toggled, mTool, &GotoOffsetTool::setIsBackwards );

    QHBoxLayout* upperOptionsLayout = new QHBoxLayout();
    upperOptionsLayout->setMargin( 0 );
    upperOptionsLayout->addWidget( mAtCursorCheckBox );
    upperOptionsLayout->addWidget( mBackwardsCheckBox );

    optionsLayout->addLayout( upperOptionsLayout );
    optionsLayout->addWidget( mExtendSelectionCheckBox );

    baseLayout->addLayout( optionsLayout );

    // Goto button
    const KGuiItem gotoGuiItem =
        KGuiItem( i18nc("@action:button","&Go"),
                  QStringLiteral("go-jump"),
                  i18nc("@info:tooltip",
                        "Go to the Offset"),
                  i18nc("@info:whatsthis",
                        "If you press the <interface>Go</interface> "
                        "button, the cursor will be moved in the document to or, "
                        "on your option, by the offset you entered above.") );
    mGotoButton = new QPushButton( this );
    KGuiItem::assign( mGotoButton, gotoGuiItem );
    connect( mGotoButton, &QPushButton::clicked, this, &GotoOffsetView::onGotoButtonClicked );
    addButton( mGotoButton, AbstractToolWidget::Default );
    baseLayout->addWidget( mGotoButton );
    baseLayout->setAlignment( mGotoButton, Qt::AlignTop );

    setTabOrder( mAddressEdit, mAtCursorCheckBox );
    setTabOrder( mAtCursorCheckBox, mBackwardsCheckBox );
    setTabOrder( mBackwardsCheckBox, mExtendSelectionCheckBox );
    setTabOrder( mExtendSelectionCheckBox, mGotoButton );

    connect( mTool, &GotoOffsetTool::isApplyableChanged,
             this, &GotoOffsetView::onApplyableChanged );

    onApplyableChanged( mTool->isApplyable() );
}


void GotoOffsetView::onApplyableChanged( bool isApplyable )
{
    // TODO: set error tooltip, like offset out of range or no document
    // TODO: set color flag to offset input
    mGotoButton->setEnabled( isApplyable );
}


void GotoOffsetView::onGotoButtonClicked()
{
    // TODO: collect recently used offset in tool instead?
    mAddressEdit->rememberCurrentAddress();

    mTool->gotoOffset();
//     emit toolUsed();
}

void GotoOffsetView::onAddressTypeChanged( int addressType )
{
    const bool isNotExpression = (mAddressEdit->format() != 2);
    if( isNotExpression
        || addressType == Okteta::AddressValidator::InvalidAddressType )
        return;

    bool fromCursor = false;
    bool backwards = false;

    if( addressType == Okteta::AddressValidator::AbsoluteAddress )
    {
        fromCursor = false;
        backwards = false; // TODO: there is no way yet for: absolute from end
    }
    else if( addressType == Okteta::AddressValidator::RelativeForwards )
    {
        fromCursor = true;
        backwards = false;
    }
    else if( addressType == Okteta::AddressValidator::RelativeBackwards )
    {
        fromCursor = true;
        backwards = true;
    }

    mAtCursorCheckBox->setChecked( fromCursor );
    mTool->setIsRelative( fromCursor );
    mBackwardsCheckBox->setChecked( backwards );
    mTool->setIsBackwards( backwards );
}

void GotoOffsetView::onFormatChanged( int formatIndex )
{
    //TODO: make sure Expr is always at index 2
    const bool isNotExpression = (formatIndex != 2);

    mAtCursorCheckBox->setEnabled( isNotExpression );
    mBackwardsCheckBox->setEnabled( isNotExpression );
}

GotoOffsetView::~GotoOffsetView() {}

}
