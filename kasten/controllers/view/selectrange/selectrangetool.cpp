/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectrangetool.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ArrayChangeMetricsList>
// KF
#include <KLocalizedString>

namespace Kasten {

SelectRangeTool::SelectRangeTool()
    : mIsEndRelative(false)
    , mIsEndBackwards(false)
{
    setObjectName(QStringLiteral("SelectRange"));
}

SelectRangeTool::~SelectRangeTool() = default;

Okteta::Address SelectRangeTool::currentSelectionStart() const
{
    return mByteArrayView ?
           mByteArrayView->startOffset() + mByteArrayView->selection().start() :
           -1;
}
Okteta::Address SelectRangeTool::currentSelectionEnd() const
{
    return mByteArrayView ?
           mByteArrayView->startOffset() + mByteArrayView->selection().end() :
           -1;
}

bool SelectRangeTool::isUsable() const
{
    return (mByteArrayView && mByteArrayModel && (mByteArrayModel->size() > 0));
}

bool SelectRangeTool::isApplyable() const
{
    const Okteta::Address start = finalTargetSelectionStart();
    const Okteta::Address end =   finalTargetSelectionEnd();

    return (mByteArrayView && mByteArrayModel
            && (start <= end)
            && (0 <= start) && (start < mByteArrayModel->size())
            && (0 <= end) && (end < mByteArrayModel->size()));
}

QString SelectRangeTool::title() const { return i18nc("@title:window of the tool to select a range", "Select"); }

void SelectRangeTool::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    const bool oldIsUsable = isUsable();
    const bool oldIsApplyable = isApplyable();

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mByteArrayView = byteArrayView;

    // TODO; check if already same document used before
    ByteArrayDocument* const document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayView && mByteArrayModel) {
        connect(mByteArrayModel, &Okteta::AbstractByteArrayModel::contentsChanged,
                this, &SelectRangeTool::onContentsChanged);
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsUsable = isUsable();
    const bool newIsApplyable = isApplyable();
    if (oldIsUsable != newIsUsable) {
        Q_EMIT isUsableChanged(newIsUsable);
    }
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setTargetStart(Okteta::Address start)
{
    const bool oldIsApplyable = isApplyable();

    mTargetStart = start;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setTargetEnd(Okteta::Address end)
{
    const bool oldIsApplyable = isApplyable();

    mTargetEnd = end;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setIsEndRelative(bool isEndRelative)
{
    const bool oldIsApplyable = isApplyable();

    mIsEndRelative = isEndRelative;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setIsEndBackwards(bool isEndBackwards)
{
    const bool oldIsApplyable = isApplyable();

    mIsEndBackwards = isEndBackwards;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::select()
{
    const Okteta::Address start = finalTargetSelectionStart();
    const Okteta::Address end =   finalTargetSelectionEnd();

    mByteArrayView->setSelection(start, end);
    mByteArrayView->setFocus();
}

Okteta::Address SelectRangeTool::finalTargetSelectionStart() const
{
    const Okteta::Address start =
        (!mByteArrayView) ?
            -1 :
        mIsEndRelative && mIsEndBackwards ?
            mTargetStart - mTargetEnd + 1 :
        /* else */
            mTargetStart;

    return start;
}

Okteta::Address SelectRangeTool::finalTargetSelectionEnd() const
{
    const Okteta::Address end =
        (!mByteArrayView) ?
            -1 :
        (!mIsEndRelative) ?
            mTargetEnd :
        mIsEndBackwards ?
            mTargetStart :
        /* else */
            mTargetStart + mTargetEnd - 1;

    return end;
}

void SelectRangeTool::onContentsChanged()
{
    // TODO: find status before content changed, e.g. by caching
    Q_EMIT isUsableChanged(isUsable());
}

}

#include "moc_selectrangetool.cpp"
