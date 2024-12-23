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
// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserErrorReport>
#include <Kasten/UserNotification>
// Okteta core
#include <Okteta/CharCodec>
// KF
#include <KComboBox>
#include <KLocalizedString>
// Qt
#include <QToolBar>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QAction>
#include <QIcon>

namespace Kasten {

CharsetConversionView::CharsetConversionView(CharsetConversionTool* tool,
                                             AbstractUserMessagesHandler* userMessagseHandler,
                                             QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
    , m_userMessagesHandler(userMessagseHandler)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // source/target charset
    auto* directionCharsetToolBar = new QToolBar(this);

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
    connect(mDirectionComboBox, qOverload<int>(&KComboBox::activated),
            mTool, &CharsetConversionTool::setConversionDirection);

    directionCharsetToolBar->addWidget(mDirectionComboBox);

    mOtherCharSetComboBox = new KComboBox(this);
    const QStringList charCodecNames = Okteta::CharCodec::codecNames();
    const int indexOfCurrentCharCodec = charCodecNames.indexOf(mTool->otherCharCodecName());
    mOtherCharSetComboBox->addItems(charCodecNames);
    mOtherCharSetComboBox->setCurrentIndex(indexOfCurrentCharCodec);
    mOtherCharSetComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    const QString targetCharsetToolTip =
        i18nc("@info:tooltip",
              "The charset the bytes are converted to.");
    mOtherCharSetComboBox->setToolTip(targetCharsetToolTip);
    const QString targetCharsetWhatsThis =
        i18nc("@info:whatsthis",
              "Select the charset the bytes are converted to.");
    mOtherCharSetComboBox->setWhatsThis(targetCharsetWhatsThis);

    connect(mOtherCharSetComboBox, &KComboBox::textActivated,
            mTool, &CharsetConversionTool::setOtherCharCodecName);

    directionCharsetToolBar->addWidget(mOtherCharSetComboBox);
    baseLayout->addWidget(directionCharsetToolBar);

    // settings
    auto* settingsBox = new QGroupBox(this);

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
    mSubstituteByteEdit->setCharCodec(mTool->targetCharCodecName());
    const QString substituteByteToolTip =
        i18nc("@info:tooltip",
              "The byte to use for chars which are not part of the target charset.");
    const QString substituteByteWhatsThis =
        i18nc("@info:whatsthis",
              "Define the byte to use for chars which are not part of the target charset.");
    mSubstituteByteEdit->setToolTip(substituteByteToolTip);
    mSubstituteByteEdit->setWhatsThis(substituteByteWhatsThis);
    mSubstituteByteEdit->setEnabled(mTool->isSubstitutingMissingChars());
    connect(mSubstituteByteEdit, &Okteta::ByteArrayComboBox::byteArrayChanged,
            this, &CharsetConversionView::onDefaultByteEditChanged);
    connect(mSubstituteByteEdit, &Okteta::ByteArrayComboBox::formatChanged,
            this, &CharsetConversionView::onDefaultByteEditCodingChanged);
    connect(mTool, &CharsetConversionTool::isSubstitutingMissingCharsChanged,
            mSubstituteByteEdit, &QWidget::setEnabled);
    connect(mTool, &CharsetConversionTool::targetCharCodecChanged,
            mSubstituteByteEdit, &Okteta::ByteArrayComboBox::setCharCodec);
    mSubstituteByteEdit->setFormat(static_cast<Okteta::ByteArrayComboBox::Coding>(mTool->substituteByteCoding()));
    mSubstituteByteEdit->setByteArray(QByteArray(1, mTool->substituteByte()));
    settingsLayout->addRow(substituteByteLabelText, mSubstituteByteEdit);
    QWidget* const substituteByteEditLabel = settingsLayout->labelForField(mSubstituteByteEdit);
    substituteByteEditLabel->setEnabled(mTool->isSubstitutingMissingChars());
    connect(mTool, &CharsetConversionTool::isSubstitutingMissingCharsChanged,
            substituteByteEditLabel, &QWidget::setEnabled);

    settingsBox->setLayout(settingsLayout);

    baseLayout->addWidget(settingsBox);

    // action
    auto* actionToolBar = new QToolBar(this);
    actionToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionToolBar->addWidget(stretcher);

    mConvertAction =
        actionToolBar->addAction(QIcon::fromTheme(QStringLiteral("run-build")),
                                 i18n("Con&vert"),
                                 this, &CharsetConversionView::onConvertButtonClicked);
    mConvertAction->setToolTip(i18nc("@info:tooltip",
                       "Converts the bytes in the selected range."));
    mConvertAction->setWhatsThis(xi18nc("@info:whatsthis",
                        "If you press the <interface>Convert</interface> button, "
                        "all bytes in the selected range "
                        "will be replaced by bytes which represent the same character "
                        "in the selected target charset."));
    baseLayout->addWidget(actionToolBar);
    baseLayout->addStretch();

    connect(mTool, &CharsetConversionTool::isApplyableChanged,
            this, &CharsetConversionView::onApplyableChanged);
    connect(mTool, &CharsetConversionTool::conversionDone,
            this, &CharsetConversionView::onConversionDone);
}

CharsetConversionView::~CharsetConversionView() = default;

void CharsetConversionView::onApplyableChanged(bool isApplyable)
{
    mConvertAction->setEnabled(isApplyable);
}

void CharsetConversionView::onDefaultByteEditChanged(const QByteArray& byteArray)
{
    const Okteta::Byte byte = byteArray.isEmpty() ? 0 : byteArray[0];
    mTool->setSubstituteByte(byte);
}

void CharsetConversionView::onDefaultByteEditCodingChanged(int coding)
{
    mTool->setSubstituteByteCoding(static_cast<CharsetConversionTool::Coding>(coding));
}

void CharsetConversionView::onConvertButtonClicked()
{
    mSubstituteByteEdit->rememberCurrentByteArray();

    mTool->convertChars();
}

void CharsetConversionView::onConversionDone(bool success, int convertedBytesCount,
                                             const std::map<Okteta::Byte, int>* failedPerByteCount)
{

    const QString messageBoxTitle = mTool->title();

    if (success) {
        QString conversionReport = (convertedBytesCount == 0) ?
                                   i18nc("@info", "No bytes converted.") :
                                   i18ncp("@info", "1 byte converted.", "%1 bytes converted.", convertedBytesCount);
        if (mTool->isSubstitutingMissingChars()) {
            int totalFailedByteCount = 0;
            for (const auto& failedByteCount : *failedPerByteCount) {
                totalFailedByteCount += failedByteCount.second;
            }

            // TODO: show table with failed bytes and their number.
            conversionReport += QLatin1String("<br />");
            conversionReport += (totalFailedByteCount == 0) ?
                                i18nc("@info", "No bytes substituted.") :
                                i18ncp("@info", "1 byte substituted.", "%1 bytes substituted.", totalFailedByteCount);
        }
        auto message = std::make_unique<Kasten::UserNotification>(mTool->targetModel(), conversionReport,
                                 messageBoxTitle);
        m_userMessagesHandler->postNotification(std::move(message));
    } else {
        // TODO: show/goto byte which on which conversion fails
        const QString errorReportText = i18nc("@info",
                                 "Conversion cancelled because of chars which are not "
                                 "in the target charset.");
        auto message = std::make_unique<Kasten::UserErrorReport>(mTool->targetModel(), errorReportText,
                                                                 messageBoxTitle);
        m_userMessagesHandler->postErrorReport(std::move(message));
    }
}

}

#include "moc_charsetconversionview.cpp"
