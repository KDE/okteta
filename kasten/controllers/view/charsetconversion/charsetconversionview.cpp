/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charsetconversionview.h"

// tool
#include "charsetconversiontool.h"
// Okteta Kasten gui
#include <bytearraycombobox.h>
// Okteta core
#include <charcodec.h>
// KDE
#include <KMessageBox>
#include <KPushButton>
#include <KComboBox>
#include <KGuiItem>
#include <KLocale>
// Qt
#include <QtGui/QFormLayout>
#include <QtGui/QLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>

#include <KDebug>

namespace Kasten2
{

CharsetConversionView::CharsetConversionView( CharsetConversionTool* tool, QWidget* parent )
  : QWidget( parent ),
    mTool( tool )
{
    QVBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // source/target charset
    QHBoxLayout* directionCharsetLayout = new QHBoxLayout();

    mDirectionComboBox = new KComboBox( this );
    const QStringList directionList = QStringList()
        << i18nc("@item:inmenu Is converted _from_ charset (selected in combobox next to this)",
                 "From")
        << i18nc("@item:inmenu Is converted _to_ charset (selected in combobox next to this)",
                 "To");
    mDirectionComboBox->addItems( directionList );
    mDirectionComboBox->setCurrentIndex( mTool->conversionDirection() );

    const QString directionToolTip =
        i18nc( "@info:tooltip",
               "The direction the bytes are converted, to or from the selected charset." );
    mDirectionComboBox->setToolTip( directionToolTip );
    const QString directionWhatsThis =
        i18nc( "@info:whatsthis",
               "Select the direction the bytes are converted, to or from the selected charset." );
    mDirectionComboBox->setWhatsThis( directionWhatsThis );
    connect( mDirectionComboBox, SIGNAL(activated(int)),
             mTool, SLOT(setConversionDirection(int)) );

    directionCharsetLayout->addWidget( mDirectionComboBox );

    mOtherCharSetComboBox = new KComboBox( this );
    const QStringList charCodecNames = Okteta::CharCodec::codecNames();
    const int indexOfCurrentCharCodec = charCodecNames.indexOf( mTool->otherCharCodecName() );
    mOtherCharSetComboBox->addItems( charCodecNames );
    mOtherCharSetComboBox->setCurrentIndex( indexOfCurrentCharCodec );

    const QString targetCharsetToolTip =
        i18nc( "@info:tooltip",
               "The charset the bytes are converted to." );
    mOtherCharSetComboBox->setToolTip( targetCharsetToolTip );
    const QString targetCharsetWhatsThis =
        i18nc( "@info:whatsthis",
               "Select the charset the bytes are converted to." );
    mOtherCharSetComboBox->setWhatsThis( targetCharsetWhatsThis );
    connect( mOtherCharSetComboBox, SIGNAL(activated(QString)),
             mTool, SLOT(setOtherCharCodecName(QString)) );

    directionCharsetLayout->addWidget( mOtherCharSetComboBox, 10 );
    baseLayout->addLayout( directionCharsetLayout );

    // settings
    QGroupBox* settingsBox = new QGroupBox( i18nc("@title:group","Parameters"), this );

    QFormLayout* settingsLayout = new QFormLayout();

    const QString substituteMissingCharLabelText =
        i18nc( "@option:check substitute bytes whose char is not part of the target charset",
                "Substitute missing:" );
    mSubstituteMissingCharCheckBox = new QCheckBox( this );
    mSubstituteMissingCharCheckBox->setChecked( mTool->isSubstitutingMissingChars() );
    const QString substituteMissingCharToolTip =
        i18nc( "@info:tooltip",
               "Selects if bytes should be substituted with a default byte "
               "if its char in the source charset is not part of the target charset." );
    const QString substituteMissingCharWhatsThis =
        i18nc( "@info:whatsthis",
               "Set to true if bytes should be substituted with a default byte "
               "if its char in the source charset is not part of the target charset." );
    mSubstituteMissingCharCheckBox->setToolTip( substituteMissingCharToolTip );
    mSubstituteMissingCharCheckBox->setWhatsThis( substituteMissingCharWhatsThis );
    connect( mSubstituteMissingCharCheckBox, SIGNAL(toggled(bool)),
             mTool, SLOT(setSubstitutingMissingChars(bool)) );
    settingsLayout->addRow( substituteMissingCharLabelText, mSubstituteMissingCharCheckBox );
    // TODO: control what happens on conflicts or unmatched chars in the target set
    // option to try only if no conflicts or unmatched chars are hit
    // choosing substitute for unmatched and resolve conflicts (general/case-by-case)
    // TODO: extra button to request check if all chars are matched, shows state
    // TODO: option to switch view to target charset, once done, if "to" other charset

    // default byte
    const QString substituteByteLabelText =
        i18nc( "@label:textbox byte to use for chars which are not part of the target charset",
               "Substitute byte:" );
    mSubstituteByteEdit = new Okteta::ByteArrayComboBox( this );
    mSubstituteByteEdit->setMinLength( 1 );
    mSubstituteByteEdit->setMaxLength( 1 );
    const QString substituteByteToolTip =
        i18nc( "@info:tooltip",
               "The byte to use for chars which are not part of the target charset." );
    const QString substituteByteWhatsThis =
        i18nc( "@info:whatsthis",
               "Define the byte to use for chars which are not part of the target charset." );
    mSubstituteByteEdit->setToolTip( substituteByteToolTip );
    mSubstituteByteEdit->setWhatsThis( substituteByteWhatsThis );
//     mSubstituteByteEdit->setEnabled( mTool->isSubstitutingMissingChars() );
    mSubstituteByteEdit->setEnabled( false ); // TODO: fix char entering and enable again
    connect( mSubstituteByteEdit, SIGNAL(byteArrayChanged(QByteArray)),
             SLOT(onDefaultByteEditChanged(QByteArray)) );
//     connect( mSubstituteMissingCharCheckBox, SIGNAL(toggled(bool)),
//              mSubstituteByteEdit, SLOT(setEnabled(bool)) );
    mSubstituteByteEdit->setByteArray( QByteArray(1, mTool->substituteByte()) );
    settingsLayout->addRow( substituteByteLabelText, mSubstituteByteEdit );

    settingsBox->setLayout( settingsLayout );

    baseLayout->addWidget( settingsBox );

    // action
    QHBoxLayout* actionsLayout = new QHBoxLayout();

    actionsLayout->addStretch();

    const KGuiItem convertGuiItem =
        KGuiItem( i18n("Con&vert"),
                  QLatin1String("run-build"),
                  i18nc("@info:tooltip",
                        "Converts the bytes in the selected range."),
                  i18nc("@info:whatsthis",
                        "If you press the <interface>Convert</interface> button, "
                        "all bytes in the selected range "
                        "will be replaced by bytes which represent the same character "
                        "in the selected target charset.") );
    mConvertButton = new KPushButton( convertGuiItem, this );
    connect( mConvertButton, SIGNAL(clicked(bool)), SLOT(onConvertButtonClicked()) );
    actionsLayout->addWidget( mConvertButton );

    baseLayout->addLayout( actionsLayout );
    baseLayout->addStretch();

    connect( mTool, SIGNAL(isApplyableChanged(bool)),
                    SLOT(onApplyableChanged(bool)) );
    connect( mTool, SIGNAL(conversionDone(bool,int,QMap<Okteta::Byte,int>)),
                    SLOT(onConversionDone(bool,int,QMap<Okteta::Byte,int>)) );
}


void CharsetConversionView::onApplyableChanged( bool isApplyable )
{
    mConvertButton->setEnabled( isApplyable );
}

void CharsetConversionView::onDefaultByteEditChanged( const QByteArray& byteArray )
{
    Q_UNUSED( byteArray );
}

void CharsetConversionView::onConvertButtonClicked()
{
    mTool->convertChars();
}

void CharsetConversionView::onConversionDone( bool success, int convertedBytesCount,
                                              const QMap<Okteta::Byte, int>& failedPerByteCount )
{

    const QString messageBoxTitle = mTool->title();

    if( success )
    {
        QString conversionReport = (convertedBytesCount==0) ?
                i18nc( "@info", "No bytes converted.") :
                i18ncp( "@info", "1 byte converted.", "%1 bytes converted.", convertedBytesCount );
        if( mTool->isSubstitutingMissingChars() )
        {
            int totalFailedByteCount = 0;
            foreach( int failedByteCount, failedPerByteCount )
                totalFailedByteCount += failedByteCount;
            //TODO: show table with failed bytes and their number.
            conversionReport += QLatin1String( "<br />" );
            conversionReport += (totalFailedByteCount==0) ?
                i18nc( "@info", "No bytes substituted.") :
                i18ncp( "@info", "1 byte substituted.", "%1 bytes substituted.", totalFailedByteCount );
        }
        KMessageBox::information( /*mParentWidget*/0,
                                  conversionReport,
                                  messageBoxTitle );
    }
    else
    {
        // TODO: show/goto byte which on which conversion fails
        KMessageBox::sorry( /*mParentWidget*/0,
                            i18nc("@info",
                                  "Conversion cancelled because of chars which are not "
                                  "in the target charset."),
                            messageBoxTitle );

    }
}

CharsetConversionView::~CharsetConversionView() {}

}
