/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2010, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddecodertool.hpp"

// lib
#include "abstractdifferentsizedialog.hpp"
// libdatatypecodecs
#include <binary8codec.hpp>
#include <octal8codec.hpp>
#include <hexadecimal8codec.hpp>
#include <uint8codec.hpp>
#include <uint16codec.hpp>
#include <uint32codec.hpp>
#include <uint64codec.hpp>
#include <sint8codec.hpp>
#include <sint16codec.hpp>
#include <sint32codec.hpp>
#include <sint64codec.hpp>
#include <float32codec.hpp>
#include <float64codec.hpp>
#include <char8codec.hpp>
#include <utf8codec.hpp>
#include <utf16codec.hpp>
// libconfigentries
#include <endiannessconfigentry.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ArrayChangeMetricsList>
#include <Okteta/ChangesDescribable>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>


namespace Kasten {

enum PODTypes
{
    BinaryId = 0,
    OctalId,
    HexadecimalId,
    Signed8BitId,
    Unsigned8BitId,
    Signed16BitId,
    Unsigned16BitId,
    Signed32BitId,
    Unsigned32BitId,
    Signed64BitId,
    Unsigned64BitId,
    Float32BitId,
    Float64BitId,
    Char8BitId,
    UTF8Id,
    UTF16Id,
    PODTypeCount
};

PODDecoderTool::PODDecoderTool()
    : mReadOnly(true)
    , mIsPodMarked(false)
    , mCharCodec(Okteta::CharCodec::createCodecForLocale())
{
    setObjectName(QStringLiteral("PODDecoder"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    const QSysInfo::Endian byteOrder = configGroup.readEntry(ByteOrderConfigKey, DefaultByteOrder);
    mPODData.setByteOrder(byteOrder);

    const bool unsignedAsHex = configGroup.readEntry(UnsignedAsHexConfigKey, DefaultUnsignedAsHex);
    mUnsignedAsHex = unsignedAsHex;

    setupDecoder();
}

PODDecoderTool::~PODDecoderTool()= default;

QString PODDecoderTool::title() const { return i18nc("@title:window", "Decoding Table"); }
bool PODDecoderTool::isReadOnly() const { return mReadOnly; }
bool PODDecoderTool::isApplyable() const { return (mByteArrayModel != nullptr); }

void PODDecoderTool::setTargetModel(AbstractModel* model)
{
    const bool oldIsApplyable = isApplyable();

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
        if (mIsPodMarked) {
            unmarkPOD();
        }
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayModel && mByteArrayView) {
        mCursorIndex = mByteArrayView->cursorPosition();
        connect(mByteArrayView, &ByteArrayView::cursorPositionChanged,
                this, &PODDecoderTool::onCursorPositionChange);
        connect(mByteArrayModel, &Okteta::AbstractByteArrayModel::contentsChanged,
                this, &PODDecoderTool::onContentsChange);
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                this, &PODDecoderTool::onCharCodecChange);
        connect(mByteArrayView, &ByteArrayView::readOnlyChanged,
                this, &PODDecoderTool::onReadOnlyChanged);
        onCharCodecChange(mByteArrayView->charCodingName());
    }

    updateData();
    onReadOnlyChanged();
    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void PODDecoderTool::setupDecoder()
{
    mTypeCodecs.resize(PODTypeCount);
    mTypeCodecs[BinaryId].reset(        new Okteta::Binary8Codec());
    mTypeCodecs[OctalId].reset(         new Okteta::Octal8Codec());
    mTypeCodecs[HexadecimalId].reset(   new Okteta::Hexadecimal8Codec());
    mTypeCodecs[Signed8BitId].reset(    new Okteta::SInt8Codec());
    mTypeCodecs[Unsigned8BitId].reset(  new Okteta::UInt8Codec());
    mTypeCodecs[Signed16BitId].reset(   new Okteta::SInt16Codec());
    mTypeCodecs[Unsigned16BitId].reset( new Okteta::UInt16Codec());
    mTypeCodecs[Signed32BitId].reset(   new Okteta::SInt32Codec());
    mTypeCodecs[Unsigned32BitId].reset( new Okteta::UInt32Codec());
    mTypeCodecs[Signed64BitId].reset(   new Okteta::SInt64Codec());
    mTypeCodecs[Unsigned64BitId].reset( new Okteta::UInt64Codec());
    mTypeCodecs[Float32BitId].reset(    new Okteta::Float32Codec());
    mTypeCodecs[Float64BitId].reset(    new Okteta::Float64Codec());
    mTypeCodecs[Char8BitId].reset(      new Okteta::Char8Codec(mCharCodec.get()));
    mTypeCodecs[UTF8Id].reset(          new Okteta::Utf8Codec());
    mTypeCodecs[UTF16Id].reset(         new Okteta::Utf16Codec());

    mDecodedValueList.resize(PODTypeCount);
    mDecodedValueByteCountList.resize(PODTypeCount);
}

void PODDecoderTool::setDifferentSizeDialog(AbstractDifferentSizeDialog* differentSizeDialog)
{
    mDifferentSizeDialog = differentSizeDialog;
}

void PODDecoderTool::setUnsignedAsHex(bool unsignedAsHex)
{
    if (mUnsignedAsHex == unsignedAsHex) {
        return;
    }

    mUnsignedAsHex = unsignedAsHex;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(UnsignedAsHexConfigKey, mUnsignedAsHex);

    updateData();
}

void PODDecoderTool::setByteOrder(int byteOrderValue)
{
    const auto byteOrder = static_cast<QSysInfo::Endian>(byteOrderValue);
    // TODO: test on no change is done in PODData, not this level
    mPODData.setByteOrder(byteOrder);

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(ByteOrderConfigKey, byteOrder);

    updateData();
}

void PODDecoderTool::onCharCodecChange(const QString& codecName)
{
    if (codecName == mCharCodec->name()) {
        return;
    }

    mCharCodec = Okteta::CharCodec::createCodec(codecName);
    static_cast<Okteta::Char8Codec*>(mTypeCodecs[Char8BitId].get())->setCharCodec(mCharCodec.get());
    updateData();
}

void PODDecoderTool::onCursorPositionChange(Okteta::Address pos)
{
    mCursorIndex = pos;
    updateData();
}

void PODDecoderTool::onContentsChange()
{
    // TODO: only update if affected
    updateData();
}

int PODDecoderTool::podCount() const { return mTypeCodecs.size(); }

QString PODDecoderTool::nameOfPOD(int podId) const
{
    return mTypeCodecs[podId]->name();
}

QVariant PODDecoderTool::value(int podId) const
{
    // TODO: add caching here
    return mDecodedValueList[podId];
}

QByteArray PODDecoderTool::bytes(int podId) const
{
    Okteta::AbstractTypeCodec* const typeCodec = mTypeCodecs[podId].get();

    // TODO: consider adding size property to AbstractTypeCodec and just get the data from mPODData
    QByteArray bytes = typeCodec->valueToBytes(mDecodedValueList[podId]);

    const int bytesSize = bytes.size();
    // need to swap the bytes?
    if ((bytesSize > 0) && (mPODData.byteOrder() != QSysInfo::ByteOrder)) {
        const int firstHalfBytesCount = bytesSize / 2;
        int j = bytesSize - 1;
        for (int i = 0; i < firstHalfBytesCount; ++i, --j) {
            const char helper = bytes[i];
            bytes[i] = bytes[j];
            bytes[j] = helper;
        }
    }

    return bytes;
}

void PODDecoderTool::setData(const QVariant& data, int podId)
{
    Okteta::AbstractTypeCodec* const typeCodec = mTypeCodecs[podId].get();

    // QVariant::operator=() only compares values' addresses for custom types,
    // so the comparison for values needs to be done by someone with knowledge about the type.
    const bool isUnchangedValue = typeCodec->areEqual(data, mDecodedValueList[podId]);

    if (isUnchangedValue) {
        return;
    }

    QByteArray bytes = typeCodec->valueToBytes(data);

    const int bytesSize = bytes.size();
    if (bytesSize == 0) {
        return;
    }

    // need to swap the bytes
    if (mPODData.byteOrder() != QSysInfo::ByteOrder) {
        const int firstHalfBytesCount = bytesSize / 2;
        int j = bytesSize - 1;
        for (int i = 0; i < firstHalfBytesCount; ++i, --j) {
            const char helper = bytes[i];
            bytes[i] = bytes[j];
            bytes[j] = helper;
        }
    }

    const int oldValueSize = mDecodedValueByteCountList[podId];
    int removedBytesSize = bytesSize;
    if (bytesSize != oldValueSize) {
//         const int sizeLeft = mByteArrayModel->size() - mCursorIndex;
        const Answer answer = Cancel; // TODO: non-persistent editor closes on new dialog -> crash after dialog
//             mDifferentSizeDialog ? mDifferentSizeDialog->query( bytesSize, oldValueSize, sizeLeft ) : Cancel;
        if (answer == Cancel) {
            return;
        }

        if (answer == AdaptSize) {
            removedBytesSize = oldValueSize;
        }
    }

    Okteta::ChangesDescribable* changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

    if (changesDescribable) {
        changesDescribable->openGroupedChange(i18nc("Edited as %datatype", "Edited as %1", typeCodec->name()));
    }
    mByteArrayModel->replace(Okteta::AddressRange::fromWidth(mCursorIndex, removedBytesSize), bytes);
    if (changesDescribable) {
        changesDescribable->closeGroupedChange();
    }
}

void PODDecoderTool::updateData()
{
    int dataSize;
    if (mByteArrayModel) {
        dataSize = mByteArrayModel->size() - mCursorIndex;
        if (dataSize > Okteta::PODData::Size) {
            dataSize = Okteta::PODData::Size;
        } else if (dataSize < 0) {
            dataSize = 0;
        }
    } else {
        dataSize = 0;
    }

    const bool hasDataSet = (dataSize > 0);
    if (hasDataSet) {
        mByteArrayModel->copyTo(mPODData.rawData(), mCursorIndex, Okteta::PODData::Size);
    }

    const bool hasChanged = mPODData.updateRawData(dataSize);

    if (!hasChanged) {
        return;
    }

    // TODO: only calculate on demand + cache
    for (int podId = 0; podId < PODTypeCount; ++podId) {
        int byteCount = 0;
        mDecodedValueList[podId] = mTypeCodecs[podId]->value(mPODData, &byteCount);
        mDecodedValueByteCountList[podId] = byteCount;
    }

    // TODO: only Q_EMIT for those strings that changed
    Q_EMIT dataChanged();
}

void PODDecoderTool::selectBytesInView(int podId)
{
    const int length = mDecodedValueByteCountList[podId];
    const Okteta::AddressRange selection = Okteta::AddressRange::fromWidth(mCursorIndex, length);
    mByteArrayView->setSelection(selection.start(), selection.end());
    mByteArrayView->setFocus();
}

void PODDecoderTool::markPOD(int podId)
{
    const int length = mDecodedValueByteCountList[podId];
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(mCursorIndex, length);
    mByteArrayView->setMarking(markingRange, true);
    mIsPodMarked = true;
}

void PODDecoderTool::unmarkPOD()
{
// TODO: marked region is property of document, not view?
    mByteArrayView->setMarking(Okteta::AddressRange());
    mIsPodMarked = false;
}

void PODDecoderTool::onReadOnlyChanged()
{
    const bool newReadOnly = ((!mByteArrayModel) || (!mByteArrayView)
                              || mByteArrayView->isReadOnly());
    if (newReadOnly != mReadOnly) {
        mReadOnly = newReadOnly;
        Q_EMIT readOnlyChanged(newReadOnly);
    }
}

}

#include "moc_poddecodertool.cpp"
