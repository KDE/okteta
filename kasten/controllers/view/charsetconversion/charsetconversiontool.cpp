/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <KLocalizedString>
// Qt
#include <QApplication>

namespace Kasten {

CharsetConversionTool::CharsetConversionTool()
{
    setObjectName(QStringLiteral("CharsetConversion"));
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
    emit isApplyableChanged(isApplyable());
}

void CharsetConversionTool::setConversionDirection(int conversionDirection)
{
    mConversionDirection = (ConversionDirection)conversionDirection;
}

void CharsetConversionTool::setSubstitutingMissingChars(bool isSubstitutingMissingChars)
{
    mSubstitutingMissingChars = isSubstitutingMissingChars;
}

void CharsetConversionTool::setSubstituteByte(int byte)
{
    mSubstituteByte = (Okteta::Byte)byte;
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
