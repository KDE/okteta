/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charsetconversiontool.hpp"

// tool
#include "charsetconversionjob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// libconfigentries
#include <bytearraycodingconfigentry.hpp>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ChangesDescribable>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QApplication>


template <>
inline Kasten::CharsetConversionTool::ConversionDirection KConfigGroup::readEntry(const char *key,
                                                                                   const Kasten::CharsetConversionTool::ConversionDirection &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Kasten::CharsetConversionTool::ConversionDirection direction =
        (entry == QLatin1String("From")) ?    Kasten::CharsetConversionTool::ConvertFrom :
        (entry == QLatin1String("To")) ?      Kasten::CharsetConversionTool::ConvertTo :
        /* else */                            defaultValue;
    return direction;
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::CharsetConversionTool::ConversionDirection &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Kasten::CharsetConversionTool::ConvertFrom) ? QLatin1String("From") : QLatin1String("To");
    writeEntry(key, valueString, flags);
}

namespace Kasten {
// helper class for KConfigGroup API
class ByteParameter
{
public:
    Okteta::Byte value;

    operator Okteta::Byte() const { return value; }
};
}

template <>
inline Kasten::ByteParameter KConfigGroup::readEntry(const char *key,
                                                     const Kasten::ByteParameter &defaultValue) const
{
    const int storageValue = readEntry(key, -1);
    if ((0 <= storageValue) && (storageValue <= 255)) {
        return Kasten::ByteParameter{static_cast<Okteta::Byte>(storageValue)};
    }
    return defaultValue;
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::ByteParameter &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    writeEntry(key, static_cast<uint>(value.value), flags);
}

template <>
inline Kasten::CharsetConversionTool::Coding
KConfigGroup::readEntry(const char *key,
                        const Kasten::CharsetConversionTool::Coding &defaultValue) const
{
    return static_cast<Kasten::CharsetConversionTool::Coding>(readEntry(key, static_cast<Kasten::ByteArrayCoding>(defaultValue)));
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::CharsetConversionTool::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    writeEntry(key, static_cast<Kasten::ByteArrayCoding>(value), flags);
}

namespace Kasten {

CharsetConversionTool::CharsetConversionTool()
{
    setObjectName(QStringLiteral("CharsetConversion"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    mOtherCharCodecName = configGroup.readEntry(OtherCharCodecNameConfigKey);
    mConversionDirection = configGroup.readEntry(ConversionDirectionConfigKey, DefaultConversionDirection);
    mSubstitutingMissingChars = configGroup.readEntry(SubstituteMissingCharsConfigKey, DefaultSubstituteMissingChars);
    mSubstituteByte = configGroup.readEntry(SubstituteByteConfigKey, ByteParameter{DefaultSubstituteByte});
    mSubstituteByteCoding = configGroup.readEntry(SubstituteByteCodingConfigKey, DefaultSubstituteByteCoding);
}

CharsetConversionTool::~CharsetConversionTool() = default;

bool CharsetConversionTool::isApplyable() const
{
    return (mByteArrayModel &&
            mByteArrayView && mByteArrayView->hasSelectedData() &&
            !mOtherCharCodecName.isEmpty() &&
            mByteArrayView->charCodingName() != mOtherCharCodecName);
}

QString CharsetConversionTool::title() const
{
    return i18nc("@title:window of the tool to convert between charsets",
                 "Charset Conversion");
}

AbstractModel* CharsetConversionTool::targetModel() const
{
    return mByteArrayView;
}

QString CharsetConversionTool::targetCharCodecName() const
{
    return (mConversionDirection == ConvertTo) ?
        mOtherCharCodecName :
        mByteArrayView ? mByteArrayView->charCodingName() : QString();
}

QString CharsetConversionTool::otherCharCodecName() const
{
    return mOtherCharCodecName;
}

CharsetConversionTool::ConversionDirection CharsetConversionTool::conversionDirection() const
{
    return mConversionDirection;
}

bool CharsetConversionTool::isSubstitutingMissingChars() const
{
    return mSubstitutingMissingChars;
}

Okteta::Byte CharsetConversionTool::substituteByte() const
{
    return mSubstituteByte;
}

CharsetConversionTool::Coding CharsetConversionTool::substituteByteCoding() const
{
    return mSubstituteByteCoding;
}


void CharsetConversionTool::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = byteArrayView;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayView && mByteArrayModel) {
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                this, &CharsetConversionTool::onViewCharCodecChanged);
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                this, &CharsetConversionTool::onViewChanged);
        connect(mByteArrayView,  &ByteArrayView::selectedDataChanged,
                this, &CharsetConversionTool::onViewChanged);

        onViewCharCodecChanged(mByteArrayView->charCodingName());
    }

    onViewChanged();
}

void CharsetConversionTool::setOtherCharCodecName(const QString& codecName)
{
    if (codecName == mOtherCharCodecName) {
        return;
    }

    mOtherCharCodecName = codecName;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(OtherCharCodecNameConfigKey, mOtherCharCodecName);

    if (mConversionDirection == ConvertTo) {
        Q_EMIT targetCharCodecChanged(mOtherCharCodecName);
    }
    Q_EMIT isApplyableChanged(isApplyable());
}

void CharsetConversionTool::setConversionDirection(int conversionDirection)
{
    if (mConversionDirection == (ConversionDirection)conversionDirection) {
        return;
    }

    const QString oldTargetCharCodecName = CharsetConversionTool::targetCharCodecName();

    mConversionDirection = (ConversionDirection)conversionDirection;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(ConversionDirectionConfigKey, mConversionDirection);

    const QString newTargetCharCodecName = CharsetConversionTool::targetCharCodecName();

    if (oldTargetCharCodecName != newTargetCharCodecName) {
        Q_EMIT targetCharCodecChanged(newTargetCharCodecName);
    }
}

void CharsetConversionTool::setSubstitutingMissingChars(bool isSubstitutingMissingChars)
{
    if (mSubstitutingMissingChars == isSubstitutingMissingChars) {
        return;
    }

    mSubstitutingMissingChars = isSubstitutingMissingChars;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(SubstituteMissingCharsConfigKey, mSubstitutingMissingChars);

    Q_EMIT isSubstitutingMissingCharsChanged(mSubstitutingMissingChars);
}

void CharsetConversionTool::setSubstituteByte(Okteta::Byte byte)
{
    if (mSubstituteByte == byte) {
        return;
    }

    mSubstituteByte = byte;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(SubstituteByteConfigKey, ByteParameter{mSubstituteByte});
}

void CharsetConversionTool::setSubstituteByteCoding(Coding coding)
{
    if (mSubstituteByteCoding == coding) {
        return;
    }

    mSubstituteByteCoding = coding;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(SubstituteByteCodingConfigKey, mSubstituteByteCoding);
}

void CharsetConversionTool::convertChars()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    const Okteta::AddressRange convertedSection = mByteArrayView->selection();
    QByteArray conversionResult;
    conversionResult.resize(convertedSection.width());

    std::unique_ptr<const Okteta::CharCodec> viewCharCodec(
        Okteta::CharCodec::createCodec(mByteArrayView->charCodingName()));
    std::unique_ptr<const Okteta::CharCodec> otherCharCodec(
        Okteta::CharCodec::createCodec(mOtherCharCodecName));
    const bool convertToOther = (mConversionDirection == ConvertTo);
    const Okteta::CharCodec* fromCharCodec = convertToOther ? viewCharCodec.get() : otherCharCodec.get();
    const Okteta::CharCodec* toCharCodec = convertToOther ? otherCharCodec.get() : viewCharCodec.get();
    auto* charsetConversionJob =
        new CharsetConversionJob(reinterpret_cast<Okteta::Byte*>(conversionResult.data()),
                                 mByteArrayModel, convertedSection,
                                 fromCharCodec,
                                 toCharCodec,
                                 mSubstitutingMissingChars, mSubstituteByte
                                 ); // TODO: report also actually converted bytes
    const bool success = charsetConversionJob->exec();

    if (success) { // TODO: if nothing needed to be converted, just report and don't add change
        auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

        if (changesDescribable) {
            const QString description =
                i18nc("@item Converted from charset 1 to charset 2",
                      "%1 to %2", fromCharCodec->name(), toCharCodec->name());
            changesDescribable->openGroupedChange(description);
        }
        std::ignore = mByteArrayModel->replace(convertedSection, conversionResult);
        if (changesDescribable) {
            changesDescribable->closeGroupedChange();
        }
    }

    viewCharCodec.reset();
    otherCharCodec.reset();

    QApplication::restoreOverrideCursor();

    const std::map<Okteta::Byte, int>& failedPerByteCount = charsetConversionJob->failedPerByteCount();
    const int convertedBytesCount = charsetConversionJob->convertedBytesCount();

    mByteArrayView->setFocus();

    Q_EMIT conversionDone(success, convertedBytesCount, &failedPerByteCount);
}

void CharsetConversionTool::onViewCharCodecChanged(const QString& charCodecName)
{
    if (mConversionDirection == ConvertFrom) {
        Q_EMIT targetCharCodecChanged(charCodecName);
    }
}

void CharsetConversionTool::onViewChanged()
{
    Q_EMIT isApplyableChanged(isApplyable());
}

}

#include "moc_charsetconversiontool.cpp"
