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


namespace Kasten {

static constexpr bool DefaultSubstituteMissingChars = false;
static constexpr Okteta::Byte DefaultSubstituteByte = 0;
static constexpr CharsetConversionTool::ConversionDirection DefaultConversionDirection = CharsetConversionTool::ConvertFrom;

static constexpr char CharsetConversionConfigGroupId[] = "CharsetConversionTool";
static constexpr char OtherCharCodecNameConfigKey[] = "OtherCharCodecName";
static constexpr char ConversionDirectionConfigKey[] = "ConversionDirection";
static constexpr char SubstituteMissingCharsConfigKey[] = "SubstituteMissingChars";
static constexpr char SubstituteByteConfigKey[] = "SubstituteByte";

CharsetConversionTool::CharsetConversionTool()
{
    setObjectName(QStringLiteral("CharsetConversion"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), CharsetConversionConfigGroupId);

    mOtherCharCodecName = configGroup.readEntry(OtherCharCodecNameConfigKey);
    mConversionDirection = configGroup.readEntry(ConversionDirectionConfigKey, DefaultConversionDirection);
    mSubstitutingMissingChars = configGroup.readEntry(SubstituteMissingCharsConfigKey, DefaultSubstituteMissingChars);
    mSubstituteByte = configGroup.readEntry(SubstituteByteConfigKey, ByteParameter{DefaultSubstituteByte});
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

void CharsetConversionTool::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayView && mByteArrayModel) {
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                this, &CharsetConversionTool::onViewChanged);
        connect(mByteArrayView,  &ByteArrayView::selectedDataChanged,
                this, &CharsetConversionTool::onViewChanged);
    }

    onViewChanged();
}

void CharsetConversionTool::setOtherCharCodecName(const QString& codecName)
{
    if (codecName == mOtherCharCodecName) {
        return;
    }

    mOtherCharCodecName = codecName;

    KConfigGroup configGroup(KSharedConfig::openConfig(), CharsetConversionConfigGroupId);
    configGroup.writeEntry(OtherCharCodecNameConfigKey, mOtherCharCodecName);

    emit isApplyableChanged(isApplyable());
}

void CharsetConversionTool::setConversionDirection(int conversionDirection)
{
    if (mConversionDirection == (ConversionDirection)conversionDirection) {
        return;
    }

    mConversionDirection = (ConversionDirection)conversionDirection;

    KConfigGroup configGroup(KSharedConfig::openConfig(), CharsetConversionConfigGroupId);
    configGroup.writeEntry(ConversionDirectionConfigKey, mConversionDirection);
}

void CharsetConversionTool::setSubstitutingMissingChars(bool isSubstitutingMissingChars)
{
    if (mSubstitutingMissingChars == isSubstitutingMissingChars) {
        return;
    }

    mSubstitutingMissingChars = isSubstitutingMissingChars;

    KConfigGroup configGroup(KSharedConfig::openConfig(), CharsetConversionConfigGroupId);
    configGroup.writeEntry(SubstituteMissingCharsConfigKey, mSubstitutingMissingChars);
}

void CharsetConversionTool::setSubstituteByte(int byte)
{
    if (mSubstituteByte == static_cast<Okteta::Byte>(byte)) {
        return;
    }

    mSubstituteByte = static_cast<Okteta::Byte>(byte);

    KConfigGroup configGroup(KSharedConfig::openConfig(), CharsetConversionConfigGroupId);
    configGroup.writeEntry(SubstituteByteConfigKey, ByteParameter{mSubstituteByte});
}

void CharsetConversionTool::convertChars()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    const Okteta::AddressRange convertedSection = mByteArrayView->selection();
    QByteArray conversionResult;
    conversionResult.resize(convertedSection.width());

    Okteta::CharCodec* viewCharCodec =
        Okteta::CharCodec::createCodec(mByteArrayView->charCodingName());
    Okteta::CharCodec* otherCharCodec =
        Okteta::CharCodec::createCodec(mOtherCharCodecName);
    const bool convertToOther = (mConversionDirection == ConvertTo);
    Okteta::CharCodec* fromCharCodec = convertToOther ? viewCharCodec : otherCharCodec;
    Okteta::CharCodec* toCharCodec = convertToOther ? otherCharCodec : viewCharCodec;
    auto* charsetConversionJob =
        new CharsetConversionJob(reinterpret_cast<Okteta::Byte*>(conversionResult.data()),
                                 mByteArrayModel, convertedSection,
                                 convertToOther ? viewCharCodec : otherCharCodec,
                                 convertToOther ? otherCharCodec : viewCharCodec,
                                 mSubstitutingMissingChars, mSubstituteByte
                                 ); // TODO: report also actually converted bytes
    const bool success = charsetConversionJob->exec();

    if (success) { // TODO: if nothing needed to be converted, just report and don't add change
        Okteta::ChangesDescribable* changesDescribable =
            qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

        if (changesDescribable) {
            const QString description =
                i18nc("Converted from charset 1 to charset 2",
                      "%1 to %2", fromCharCodec->name(), toCharCodec->name());
            changesDescribable->openGroupedChange(description);
        }
        mByteArrayModel->replace(convertedSection, conversionResult);
        if (changesDescribable) {
            changesDescribable->closeGroupedChange();
        }
    }

    delete viewCharCodec;
    delete otherCharCodec;

    QApplication::restoreOverrideCursor();

    const QMap<Okteta::Byte, int>& failedPerByteCount = charsetConversionJob->failedPerByteCount();
    const int convertedBytesCount = charsetConversionJob->convertedBytesCount();

    mByteArrayView->setFocus();

    emit conversionDone(success, convertedBytesCount, failedPerByteCount);
}

void CharsetConversionTool::onViewChanged()
{
    emit isApplyableChanged(isApplyable());
}

}
