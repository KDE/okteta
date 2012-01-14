/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofileedit.h"

// Okteta Gui Kasten
#include <bytearrayviewprofile.h>
// Okteta core
#include <charcodec.h>
// KDE
#include <KLocale>
#include <KIntNumInput>
#include <KComboBox>
// Qt
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QFormLayout>
#include <QtGui/QCheckBox>
#include <KLineEdit>

namespace Kasten2
{

static int
listIndexFromByteArrayCodingsFlags( int byteArrayCodingsFlags )
{
    return byteArrayCodingsFlags - 1;
}

static int
byteArrayCodingsFlagsFromListIndex( int listIndex )
{
    return listIndex + 1;
}


ViewProfileEdit::ViewProfileEdit( QWidget* parent )
  : QWidget( parent )
{
    QVBoxLayout* layout = new QVBoxLayout( this );

    // titel
    QFormLayout* titleFormLayout = new QFormLayout;
    // char for non-printable bytes
    mTitleEdit = new KLineEdit( this );
    titleFormLayout->addRow( i18n("Title:"), mTitleEdit );

    // display settings
    QGroupBox* displayBox = new QGroupBox( this );
    displayBox->setTitle( i18n("Display") );
    QFormLayout* displayBoxFormLayout = new QFormLayout( displayBox );
    // line offset shown
    mLineOffsetShownCheckBox = new QCheckBox( displayBox );
    displayBoxFormLayout->addRow( i18n("Show Line Offset:"), mLineOffsetShownCheckBox );
    // values or char shown
    mValuesCharsShownComboBox = new KComboBox( displayBox );
    QStringList valuesCharsList;
    valuesCharsList.append( i18nc("@item:","Values") );
    valuesCharsList.append( i18nc("@item:","Chars") );
    valuesCharsList.append( i18nc("@item:","Values & Chars") );
    mValuesCharsShownComboBox->addItems( valuesCharsList );
    displayBoxFormLayout->addRow( i18n("Show Values or Chars:"), mValuesCharsShownComboBox );
    // display mode
    const QString displayModeLabel =
        i18nc( "@label:listbox ",
               "Show with Rows or Columns:" );
    mDisplayModeComboBox = new KComboBox( displayBox );
    QStringList displayModeList;
    displayModeList.append( i18nc("@item:","Columns") );
    displayModeList.append( i18nc("@item:","Rows") );
    mDisplayModeComboBox->addItems( displayModeList );
    displayBoxFormLayout->addRow( displayModeLabel, mDisplayModeComboBox );

    // layout settings
    QGroupBox* layoutBox = new QGroupBox( this );
    layoutBox->setTitle( i18n("Layout") );
    QFormLayout* layoutBoxFormLayout = new QFormLayout( layoutBox );
    // line break
    mLineBreakComboBox = new KComboBox( layoutBox );
    QStringList lineBreakList;
    lineBreakList.append( i18nc("@item:inmenu  The layout will not change on size changes.",
                                "Off") );
    lineBreakList.append( i18nc("@item:inmenu  The layout will adapt to the size, but only with complete groups of bytes.",
                                "Wrap Only Complete Byte Groups") );
    lineBreakList.append( i18nc("@item:inmenu  The layout will adapt to the size and fit in as much bytes per line as possible.",
                                "On") );
    mLineBreakComboBox->addItems( lineBreakList );
    connect( mLineBreakComboBox, SIGNAL(currentIndexChanged(int)),
             SLOT(onLineBreakIndexChanged(int)) );
    layoutBoxFormLayout->addRow( i18n("Break lines:"), mLineBreakComboBox );
    // bytes per group
    mGroupedBytesCountEdit = new KIntNumInput( this );
    mGroupedBytesCountEdit->setRange( 0, INT_MAX );
    mGroupedBytesCountEdit->setSuffix( ki18np(" byte"," bytes") );
    const QString noGroupingText = i18nc( "@label",
                                          "No grouping." );
    mGroupedBytesCountEdit->setSpecialValueText( noGroupingText );
    const QString groupedBytesCountLabel =
        i18nc( "@label:spinbox number of bytes which are grouped",
               "Bytes per Group:" );
    layoutBoxFormLayout->addRow( groupedBytesCountLabel, mGroupedBytesCountEdit );
    // bytes per group
    mBytesPerLineEdit = new KIntNumInput( this );
    mBytesPerLineEdit->setRange( 1, INT_MAX );
    mBytesPerLineEdit->setSuffix( ki18np(" byte"," bytes") );
    const QString bytesPerLineLabel =
        i18nc( "@label:spinbox number of bytes which are shown per line",
                "Bytes per Line:" );
    layoutBoxFormLayout->addRow( bytesPerLineLabel, mBytesPerLineEdit );

    // value settings
    QGroupBox* valuesBox = new QGroupBox( this );
    valuesBox->setTitle( i18n("Values") );
    QFormLayout* valuesBoxFormLayout = new QFormLayout( valuesBox );
    // coding
    mValueCodingComboBox = new KComboBox( valuesBox );
    QStringList valueCodingList;
    valueCodingList.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format",
                                  "Hexadecimal") );
    valueCodingList.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",
                                  "Decimal")     );
    valueCodingList.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",
                                  "Octal")       );
    valueCodingList.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",
                                  "Binary")      );
    mValueCodingComboBox->addItems( valueCodingList );
    valuesBoxFormLayout->addRow( i18n("Coding:"), mValueCodingComboBox );

    // char settings
    QGroupBox* charsBox = new QGroupBox( this );
    charsBox->setTitle( i18n("Chars") );
    QFormLayout* charsBoxFormLayout = new QFormLayout( charsBox );
    // coding
    mCharCodingComboBox = new KComboBox( charsBox );
    mCharCodingComboBox->addItems( Okteta::CharCodec::codecNames() );
    charsBoxFormLayout->addRow( i18n("Coding:"), mCharCodingComboBox );
    // line offset shown
    mNonPrintableShownCheckBox = new QCheckBox( charsBox );
    charsBoxFormLayout->addRow( i18n("Show Non-printable:"), mNonPrintableShownCheckBox );
    // char for non-printable bytes
    mNonPrintableCharEdit = new KLineEdit( charsBox ); // TODO: use a validator to ensure always one char
    mNonPrintableCharEdit->setMaxLength( 1 );
    charsBoxFormLayout->addRow( i18n("Char for non-printable bytes:"), mNonPrintableCharEdit );
    // char for undefined bytes
    mUndefinedCharEdit = new KLineEdit( charsBox ); // TODO: use a validator to ensure always one char
    mUndefinedCharEdit->setMaxLength( 1 );
    charsBoxFormLayout->addRow( i18n("Char for undefined bytes:"), mUndefinedCharEdit );

    layout->addLayout( titleFormLayout );
    layout->addWidget( displayBox );
    layout->addWidget( layoutBox );
    layout->addWidget( valuesBox );
    layout->addWidget( charsBox );
}

ByteArrayViewProfile ViewProfileEdit::viewProfile() const
{
    ByteArrayViewProfile viewProfile;
    viewProfile.setViewProfileTitle( mTitleEdit->text() );

    viewProfile.setOffsetColumnVisible( mLineOffsetShownCheckBox->isChecked() );
    const int visibleByteArrayCodings =
        byteArrayCodingsFlagsFromListIndex( mValuesCharsShownComboBox->currentIndex() );
    viewProfile.setVisibleByteArrayCodings( visibleByteArrayCodings );
    viewProfile.setViewModus( mDisplayModeComboBox->currentIndex() );

    viewProfile.setLayoutStyle( mLineBreakComboBox->currentIndex() );
    viewProfile.setNoOfGroupedBytes( mGroupedBytesCountEdit->value() );
    viewProfile.setNoOfBytesPerLine( mBytesPerLineEdit->value() );

    viewProfile.setValueCoding( mValueCodingComboBox->currentIndex() );

    viewProfile.setCharCoding( mCharCodingComboBox->currentText() );
    viewProfile.setShowsNonprinting( mNonPrintableShownCheckBox->isChecked() );
    viewProfile.setSubstituteChar( mNonPrintableCharEdit->text().at(0) ); // TODO: need make sure is one char
    viewProfile.setUndefinedChar( mUndefinedCharEdit->text().at(0) ); // TODO: need make sure is one char

    return viewProfile;
}

void ViewProfileEdit::setViewProfile( const ByteArrayViewProfile& viewProfile )
{
    mTitleEdit->setText( viewProfile.viewProfileTitle() );

    mLineOffsetShownCheckBox->setChecked( viewProfile.offsetColumnVisible() );
    const int valuesCharsShownListIndex =
        listIndexFromByteArrayCodingsFlags( viewProfile.visibleByteArrayCodings() );
    mValuesCharsShownComboBox->setCurrentIndex( valuesCharsShownListIndex );
    mDisplayModeComboBox->setCurrentIndex( viewProfile.viewModus() );

    mLineBreakComboBox->setCurrentIndex( viewProfile.layoutStyle() );
    mGroupedBytesCountEdit->setValue( viewProfile.noOfGroupedBytes() );
    mBytesPerLineEdit->setValue( viewProfile.noOfBytesPerLine() );

    mValueCodingComboBox->setCurrentIndex( viewProfile.valueCoding() );

    mCharCodingComboBox->setCurrentItem( viewProfile.charCodingName() );
    mNonPrintableShownCheckBox->setChecked( viewProfile.showsNonprinting() );
    mNonPrintableCharEdit->setText( viewProfile.substituteChar() );
    mUndefinedCharEdit->setText( viewProfile.undefinedChar() );
}

void ViewProfileEdit::onLineBreakIndexChanged( int lineBreakIndex )
{
    const bool isLineBreakByByte = ( lineBreakIndex == 0 );

    mBytesPerLineEdit->setEnabled( isLineBreakByByte );
}

ViewProfileEdit::~ViewProfileEdit()
{
}

}
