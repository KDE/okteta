/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charsetconversionview.hpp"

// tool
#include "charsetconversiontool.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>
// Okteta core
#include <Okteta/CharCodec>
// KF
#include <KMessageBox>
#include <KComboBox>
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QPushButton>
#include <QFormLayout>
#include <QLayout>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>

namespace Kasten {

CharsetConversionView::CharsetConversionView(CharsetConversionTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // source/target charset
    auto* directionCharsetLayout = new QHBoxLayout();

    mDirectionComboBox = new KComboBox(this);
    const QStringList directionList {
        i18nc("@item:inmenu Is converted _from_ charset (selected in combobox next to this)",
              "From"),
        i18nc("@item:inmenu Is converted _to_ charset (selected in combobox next to this)",
              "To"),
    };
    mDirectionComboBox->addItems(directionList);
    mDirectionComboBox->setCurrentIndex(mTool->conversionDirection());

    const QString directionToolTip =
        i18nc("@info:tooltip",
              "The direction the bytes are converted, to or from the selected charset.");
    mDirectionComboBox->setToolTip(directionToolTip);
    const QString directionWhatsThis =
        i18nc("@info:whatsthis",
              "Select the direction the bytes are converted, to or from the selected charset.");
    mDirectionComboBox->setWhatsThis(directionWhatsThis);
    connect(mDirectionComboBox, QOverload<int>::of(&KComboBox::activated),
            mTool, &CharsetConversionTool::setConversionDirection);

    directionCharsetLayout->addWidget(mDirectionComboBox);

    mOtherCharSetComboBox = new KComboBox(this);
    const QStringList charCodecNames = Okteta::CharCodec::codecNames();
    const int indexOfCurrentCharCodec = charCodecNames.indexOf(mTool->otherCharCodecName());
    mOtherCharSetComboBox->addItems(charCodecNames);
    mOtherCharSetComboBox->setCurrentIndex(indexOfCurrentCharCodec);

    const QString targetCharsetToolTip =
        i18nc("@info:tooltip",
              "The charset the bytes are converted to.");
    mOtherCharSetComboBox->setToolTip(targetCharsetToolTip);
    const QString targetCharsetWhatsThis =
        i18nc("@info:whatsthis",
              "Select the charset the bytes are converted to.");
    mOtherCharSetComboBox->setWhatsThis(targetCharsetWhatsThis);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    connect(mOtherCharSetComboBox, &KComboBox::textActivated,
            mTool, &CharsetConversionTool::setOtherCharCodecName);
#else
    connect(mOtherCharSetComboBox, QOverload<const QString&>::of(&KComboBox::activated),
            mTool, &CharsetConversionTool::setOtherCharCodecName);
#endif

    directionCharsetLayout->addWidget(mOtherCharSetComboBox, 10);
    baseLayout->addLayout(directionCharsetLayout);

    // settings
    QGroupBox* settingsBox = new QGroupBox(i18nc("@title:group", "Parameters"), this);

    auto* settingsLayout = new QFormLayout();

    const QString substituteMissingCharLabelText =
        i18nc("@option:check substitute bytes whose char is not part of the target charset",
              "Substitute missing:");
    mSubstituteMissingCharCheckBox = new QCheckBox(this);
    mSubstituteMissingCharCheckBox->setChecked(mTool->isSubstitutingMissingChars());
    const QString substituteMissingCharToolTip =
        i18nc("@info:tooltip",
              "Selects if bytes should be substituted with a default byte "
              "if its char in the source charset is not part of the target charset.");
    const QString substituteMissingCharWhatsThis =
        i18nc("@info:whatsthis",
              "Set to true if bytes should be substituted with a default byte "
              "if its char in the source charset is not part of the target charset.");
    mSubstituteMissingCharCheckBox->setToolTip(substituteMissingCharToolTip);
    mSubstituteMissingCharCheckBox->setWhatsThis(substituteMissingCharWhatsThis);
    connect(mSubstituteMissingCharCheckBox, &QCheckBox::toggled,
            mTool, &CharsetConversionTool::setSubstitutingMissingChars);
    settingsLayout->addRow(substituteMissingCharLabelText, mSubstituteMissingCharCheckBox);
    // TODO: control what happens on conflicts or unmatched chars in the target set
    // option to try only if no conflicts or unmatched chars are hit
    // choosing substitute for unmatched and resolve conflicts (general/case-by-case)
    // TODO: extra button to request check if all chars are matched, shows state
    // TODO: option to switch view to target charset, once done, if "to" other charset

    // default byte
    const QString substituteByteLabelText =
        i18nc("@label:textbox byte to use for chars which are not part of the target charset",
              "Substitute byte:");
    mSubstituteByteEdit = new Okteta::ByteArrayComboBox(this);
    mSubstituteByteEdit->setMinLength(1);
    mSubstituteByteEdit->setMaxLength(1);
    const QString substituteByteToolTip =
        i18nc("@info:tooltip",
              "The byte to use for chars which are not part of the target charset.");
    const QString substituteByteWhatsThis =
        i18nc("@info:whatsthis",
              "Define the byte to use for chars which are not part of the target charset.");
    mSubstituteByteEdit->setToolTip(substituteByteToolTip);
    mSubstituteByteEdit->setWhatsThis(substituteByteWhatsThis);
//     mSubstituteByteEdit->setEnabled( mTool->isSubstitutingMissingChars() );
    mSubstituteByteEdit->setEnabled(false);   // TODO: fix char entering and enable again
    connect(mSubstituteByteEdit, &Okteta::ByteArrayComboBox::byteArrayChanged,
            this, &CharsetConversionView::onDefaultByteEditChanged);
//     connect( mSubstituteMissingCharCheckBox, SIGNAL(toggled(bool)),
//              mSubstituteByteEdit, SLOT(setEnabled(bool)) );
    mSubstituteByteEdit->setByteArray(QByteArray(1, mTool->substituteByte()));
    settingsLayout->addRow(substituteByteLabelText, mSubstituteByteEdit);

    settingsBox->setLayout(settingsLayout);

    baseLayout->addWidget(settingsBox);

    // action
    auto* actionsLayout = new QHBoxLayout();

    actionsLayout->addStretch();

    const KGuiItem convertGuiItem =
        KGuiItem(i18n("Con&vert"),
                 QStringLiteral("run-build"),
                 i18nc("@info:tooltip",
                       "Converts the bytes in the selected range."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Convert</interface> button, "
                        "all bytes in the selected range "
                        "will be replaced by bytes which represent the same character "
                        "in the selected target charset."));
    mConvertButton = new QPushButton(this);
    KGuiItem::assign(mConvertButton, convertGuiItem);
    connect(mConvertButton, &QPushButton::clicked, this, &CharsetConversionView::onConvertButtonClicked);
    actionsLayout->addWidget(mConvertButton);

    baseLayout->addLayout(actionsLayout);
    baseLayout->addStretch();

    connect(mTool, &CharsetConversionTool::isApplyableChanged,
            this, &CharsetConversionView::onApplyableChanged);
    connect(mTool, &CharsetConversionTool::conversionDone,
            this, &CharsetConversionView::onConversionDone);
}

CharsetConversionView::~CharsetConversionView() = default;

void CharsetConversionView::onApplyableChanged(bool isApplyable)
{
    mConvertButton->setEnabled(isApplyable);
}

void CharsetConversionView::onDefaultByteEditChanged(const QByteArray& byteArray)
{
    Q_UNUSED(byteArray);
}

void CharsetConversionView::onConvertButtonClicked()
{
    mTool->convertChars();
}

void CharsetConversionView::onConversionDone(bool success, int convertedBytesCount,
                                             const QMap<Okteta::Byte, int>& failedPerByteCount)
{

    const QString messageBoxTitle = mTool->title();

    if (success) {
        QString conversionReport = (convertedBytesCount == 0) ?
                                   i18nc("@info", "No bytes converted.") :
                                   i18ncp("@info", "1 byte converted.", "%1 bytes converted.", convertedBytesCount);
        if (mTool->isSubstitutingMissingChars()) {
            int totalFailedByteCount = 0;
            for (int failedByteCount : failedPerByteCount) {
                totalFailedByteCount += failedByteCount;
            }

            // TODO: show table with failed bytes and their number.
            conversionReport += QLatin1String("<br />");
            conversionReport += (totalFailedByteCount == 0) ?
                                i18nc("@info", "No bytes substituted.") :
                                i18ncp("@info", "1 byte substituted.", "%1 bytes substituted.", totalFailedByteCount);
        }
        KMessageBox::information(/*mParentWidget*/ nullptr,
                                 conversionReport,
                                 messageBoxTitle);
    } else {
        // TODO: show/goto byte which on which conversion fails
        KMessageBox::sorry(/*mParentWidget*/ nullptr,
                           i18nc("@info",
                                 "Conversion cancelled because of chars which are not "
                                 "in the target charset."),
                           messageBoxTitle);

    }
}

}
