/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

int SelectRangeTool::currentSelectionStart() const
{
    return mByteArrayView ?
           mByteArrayView->startOffset() + mByteArrayView->selection().start() :
           -1;
}
int SelectRangeTool::currentSelectionEnd() const
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
    const int start = finalTargetSelectionStart();
    const int end =   finalTargetSelectionEnd();

    return (mByteArrayView && mByteArrayModel
            && (start <= end)
            && (0 <= start) && (start < mByteArrayModel->size())
            && (0 <= end) && (end < mByteArrayModel->size()));
}

QString SelectRangeTool::title() const { return i18nc("@title:window of the tool to select a range", "Select"); }

void SelectRangeTool::setTargetModel(AbstractModel* model)
{
    const bool oldIsUsable = isUsable();
    const bool oldIsApplyable = isApplyable();

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    ByteArrayDocument* document =
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
        emit isUsableChanged(newIsUsable);
    }
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setTargetStart(Okteta::Address start)
{
    const bool oldIsApplyable = isApplyable();

    mTargetStart = start;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setTargetEnd(Okteta::Address end)
{
    const bool oldIsApplyable = isApplyable();

    mTargetEnd = end;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setIsEndRelative(bool isEndRelative)
{
    const bool oldIsApplyable = isApplyable();

    mIsEndRelative = isEndRelative;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::setIsEndBackwards(bool isEndBackwards)
{
    const bool oldIsApplyable = isApplyable();

    mIsEndBackwards = isEndBackwards;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SelectRangeTool::select()
{
    const int start = finalTargetSelectionStart();
    const int end =   finalTargetSelectionEnd();

    mByteArrayView->setSelection(start, end);
    mByteArrayView->setFocus();
}

int SelectRangeTool::finalTargetSelectionStart() const
{
    const int start =
        (!mByteArrayView) ?
            -1 :
        mIsEndRelative && mIsEndBackwards ?
            mTargetStart - mTargetEnd + 1 :
        /* else */
            mTargetStart;

    return start;
}

int SelectRangeTool::finalTargetSelectionEnd() const
{
    const int end =
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
    emit isUsableChanged(isUsable());
}

}
