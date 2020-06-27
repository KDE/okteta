/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <KLocalizedString>
// Qt
#include <QApplication>

namespace Kasten {

static constexpr int DefaultMinLength = 3;

StringsExtractTool::StringsExtractTool()
    : mExtractedStringsUptodate(false)
    , mSourceByteArrayModelUptodate(false)
    , mMinLength(DefaultMinLength)
{
    setObjectName(QStringLiteral("Strings"));
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

// TODO: add model with offset and string
// doubleclick moves cursor to offset
// filter für Suche, inkl. Regulärausdrücke
// groß/kleinschreibung
// voll strings, auch mit Leerzeichen
void StringsExtractTool::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

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
    emit uptodateChanged(mExtractedStringsUptodate);
    emit isApplyableChanged(isApplyable());
    emit canHighlightStringChanged(canHighlightString());
    if (mSourceByteArrayModel == mByteArrayModel && mByteArrayView) {
        emit offsetCodingChanged(mByteArrayView->offsetCoding());
    }
}

int StringsExtractTool::offsetCoding() const { return (mByteArrayView ? mByteArrayView->offsetCoding() : 0); }

void StringsExtractTool::setMinLength(int minLength)
{
    mMinLength = minLength;
    checkUptoDate();
    emit uptodateChanged(mExtractedStringsUptodate);
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

void StringsExtractTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    checkUptoDate();
    emit uptodateChanged(mExtractedStringsUptodate);
    emit isApplyableChanged(isApplyable());
}

void StringsExtractTool::onSourceChanged()
{
    mExtractedStringsUptodate = false;
    mSourceByteArrayModelUptodate = false;
    emit uptodateChanged(false);
    emit canHighlightStringChanged(false);
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

    Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec(mByteArrayView->charCodingName());
    ExtractStringsJob* extractStringsJob =
        new ExtractStringsJob(mByteArrayModel, mByteArrayView->selection(), charCodec, mMinLength,
                              &mContainedStringList);
    extractStringsJob->exec();
    delete charCodec;

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
    emit uptodateChanged(true);
    emit canHighlightStringChanged(true);
    emit offsetCodingChanged(mByteArrayView->offsetCoding());
}

}
