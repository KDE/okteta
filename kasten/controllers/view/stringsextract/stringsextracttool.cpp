/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2012, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextracttool.hpp"

// lib
#include "extractstringsjob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ArrayChangeMetricsList>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QApplication>

namespace Kasten {

StringsExtractTool::StringsExtractTool()
    : mExtractedStringsUptodate(false)
    , mSourceByteArrayModelUptodate(false)
{
    setObjectName(QStringLiteral("Strings"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    mMinLength = configGroup.readEntry(MinimumLengthConfigKey, DefaultMinLength);
}

StringsExtractTool::~StringsExtractTool() = default;

bool StringsExtractTool::isApplyable() const
{
    return (mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData() && mMinLength > 0);
}

bool StringsExtractTool::canHighlightString() const
{
    return (mSourceByteArrayModel == mByteArrayModel
            && mByteArrayView && mSourceByteArrayModelUptodate);
}

QString StringsExtractTool::title() const { return i18nc("@title:window of the tool to extract strings", "Strings"); }

AbstractModel* StringsExtractTool::targetModel() const
{
    return mByteArrayView;
}

// TODO: add model with offset and string
// doubleclick moves cursor to offset
// filter für Suche, inkl. Regulärausdrücke
// groß/kleinschreibung
// voll strings, auch mit Leerzeichen
void StringsExtractTool::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    if (mByteArrayView) {
        // disconnect explicitly from all connects but QObject::detroyed,
        // for the case mSourceByteArrayView is mByteArrayView
        disconnect(mByteArrayView,  &ByteArrayView::selectedDataChanged,
                   this, &StringsExtractTool::onSelectionChanged);
        disconnect(mByteArrayView, &ByteArrayView::offsetCodingChanged,
                   this, &StringsExtractTool::offsetCodingChanged);
    }

    mByteArrayView = byteArrayView;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayView && mByteArrayModel) {
        connect(mByteArrayView,  &ByteArrayView::selectedDataChanged,
                this, &StringsExtractTool::onSelectionChanged);

        // if strings are from same model, adapt offsetcoding
        if (mSourceByteArrayModel == mByteArrayModel) {
            connect(mByteArrayView, &ByteArrayView::offsetCodingChanged,
                    this, &StringsExtractTool::offsetCodingChanged);
        }
    }

    // TODO: if there is no view, there is nothing to extract.
    // or this could be the view where we got the strings from and it did not change
    checkUptoDate();
    Q_EMIT uptodateChanged(mExtractedStringsUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
    Q_EMIT canHighlightStringChanged(canHighlightString());
    if (mSourceByteArrayModel == mByteArrayModel && mByteArrayView) {
        Q_EMIT offsetCodingChanged(mByteArrayView->offsetCoding());
    }
}

int StringsExtractTool::offsetCoding() const { return (mByteArrayView ? mByteArrayView->offsetCoding() : 0); }

void StringsExtractTool::setMinLength(int minLength)
{
    if (mMinLength == minLength) {
        return;
    }

    mMinLength = minLength;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(MinimumLengthConfigKey, mMinLength);

    checkUptoDate();
    Q_EMIT uptodateChanged(mExtractedStringsUptodate);
}

void StringsExtractTool::checkUptoDate()
{
    mExtractedStringsUptodate =
        (mSourceByteArrayModel == mByteArrayModel
         && mByteArrayView && mSourceSelection == mByteArrayView->selection()
         && mSourceMinLength == mMinLength
         && mSourceByteArrayModelUptodate);
}

void StringsExtractTool::markString(int stringId)
{
    if (mSourceByteArrayView != mByteArrayView) {
        if (mSourceByteArrayView) {
            mSourceByteArrayView->disconnect(this);
        }
        mSourceByteArrayView = mByteArrayView;
        connect(mSourceByteArrayView,  &ByteArrayView::destroyed,
                this, &StringsExtractTool::onSourceViewDestroyed);
    }
    const ContainedString& containedString = mContainedStringList.at(stringId);
    const Okteta::Address offset = containedString.offset();
    const int length = containedString.string().length();
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(offset, length);
    mSourceByteArrayView->setMarking(markingRange, true);
}

void StringsExtractTool::unmarkString()
{
// TODO: marked region is property of document, not view?
    if (mSourceByteArrayView) {
        mSourceByteArrayView->setMarking(Okteta::AddressRange());
    }
}

void StringsExtractTool::selectString(int stringId)
{
    if (mSourceByteArrayView != mByteArrayView) {
        if (mSourceByteArrayView) {
            mSourceByteArrayView->disconnect(this);
        }
        mSourceByteArrayView = mByteArrayView;
        connect(mSourceByteArrayView,  &ByteArrayView::destroyed,
                this, &StringsExtractTool::onSourceViewDestroyed);
    }
    const ContainedString& containedString = mContainedStringList.at(stringId);
    const Okteta::Address offset = containedString.offset();
    const int length = containedString.string().length();
    const Okteta::AddressRange selection = Okteta::AddressRange::fromWidth(offset, length);
    mSourceByteArrayView->setSelection(selection.start(), selection.end());
    mSourceByteArrayView->setFocus();
}

void StringsExtractTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    checkUptoDate();
    Q_EMIT uptodateChanged(mExtractedStringsUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
}

void StringsExtractTool::onSourceChanged()
{
    mExtractedStringsUptodate = false;
    mSourceByteArrayModelUptodate = false;
    Q_EMIT uptodateChanged(false);
    Q_EMIT canHighlightStringChanged(false);
}

void StringsExtractTool::onSourceDestroyed()
{
    mSourceByteArrayModel = nullptr;
    onSourceChanged();
}

void StringsExtractTool::onSourceViewDestroyed()
{
    mSourceByteArrayView = nullptr;
}

// TODO: use TextByteArrayAnalyzer
void StringsExtractTool::extractStrings()
{
    // forget old string source
    if (mSourceByteArrayModel) {
        mSourceByteArrayModel->disconnect(this);
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto charCodec = Okteta::CharCodec::createCodec(mByteArrayView->charCodingName());
    auto* extractStringsJob =
        new ExtractStringsJob(mByteArrayModel, mByteArrayView->selection(), charCodec.get(), mMinLength,
                              &mContainedStringList);
    extractStringsJob->exec();
    charCodec.reset();

    QApplication::restoreOverrideCursor();

    // remember new string source
    mSourceByteArrayModel = mByteArrayModel;
    mSourceSelection = mByteArrayView->selection();
    mSourceMinLength = mMinLength;
    connect(mSourceByteArrayModel,  &Okteta::AbstractByteArrayModel::contentsChanged,
            this, &StringsExtractTool::onSourceChanged);
    connect(mSourceByteArrayModel,  &Okteta::AbstractByteArrayModel::destroyed,
            this, &StringsExtractTool::onSourceDestroyed);
    connect(mByteArrayView, &ByteArrayView::offsetCodingChanged,
            this, &StringsExtractTool::offsetCodingChanged);

    mExtractedStringsUptodate = true;
    mSourceByteArrayModelUptodate = true;
    Q_EMIT uptodateChanged(true);
    Q_EMIT canHighlightStringChanged(true);
    Q_EMIT offsetCodingChanged(mByteArrayView->offsetCoding());
    Q_EMIT extractionDone(mContainedStringList.size());
}

}

#include "moc_stringsextracttool.cpp"
