/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "gotooffsettool.hpp"

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

GotoOffsetTool::GotoOffsetTool()
{
    setObjectName(QStringLiteral("GotoOffset"));
}

GotoOffsetTool::~GotoOffsetTool() = default;

int GotoOffsetTool::currentOffset() const
{
    return mByteArrayView ?
           mByteArrayView->startOffset() + mByteArrayView->cursorPosition() :
           -1;
}

bool GotoOffsetTool::isUsable() const
{
    return (mByteArrayView && mByteArrayModel && (mByteArrayModel->size() > 0));
}

bool GotoOffsetTool::isApplyable() const
{
    const int newPosition = finalTargetOffset();

    return (mByteArrayView && mByteArrayModel
            && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()));
}

QString GotoOffsetTool::title() const { return i18nc("@title:window of the tool to set a new offset for the cursor", "Goto"); }

void GotoOffsetTool::setTargetModel(AbstractModel* model)
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
                this, &GotoOffsetTool::onContentsChanged);
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

void GotoOffsetTool::setTargetOffset(Okteta::Address targetOffset)
{
    const bool oldIsApplyable = isApplyable();

    mTargetOffset = targetOffset;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void GotoOffsetTool::setIsRelative(bool isRelative)
{
    const bool oldIsApplyable = isApplyable();

    mIsRelative = isRelative;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void GotoOffsetTool::setIsSelectionToExtent(bool isSelectionToExtent)
{
    const bool oldIsApplyable = isApplyable();

    mIsSelectionToExtent = isSelectionToExtent;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void GotoOffsetTool::setIsBackwards(bool isBackwards)
{
    const bool oldIsApplyable = isApplyable();

    mIsBackwards = isBackwards;

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void GotoOffsetTool::gotoOffset()
{
    const int newPosition = finalTargetOffset();

    if (mIsSelectionToExtent) {
        mByteArrayView->setSelectionCursorPosition(newPosition);
    } else {
        mByteArrayView->setCursorPosition(newPosition);
    }
    mByteArrayView->setFocus();
}

int GotoOffsetTool::finalTargetOffset() const
{
    const int newPosition =
        (!mByteArrayView) ? -1 :
        mIsRelative ?
            (mIsBackwards ? mByteArrayView->cursorPosition() - mTargetOffset :
                            mByteArrayView->cursorPosition() + mTargetOffset) :
            (mIsBackwards ? mByteArrayModel->size() - mTargetOffset :
                            mTargetOffset);

    return newPosition;
}

void GotoOffsetTool::onContentsChanged()
{
    // TODO: find status before content changed, e.g. by caching
    emit isUsableChanged(isUsable());
}

}
