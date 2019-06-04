/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "replacetool.hpp"

// controller
#include "replacejob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

namespace Kasten {

ReplaceTool::ReplaceTool()
{
    setObjectName(QStringLiteral("Replace"));
}

ReplaceTool::~ReplaceTool() = default;

bool ReplaceTool::isApplyable() const
{
    return (mByteArrayView && mByteArrayModel && !mByteArrayView->isReadOnly()) && !mReplaceJob;
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayView && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString ReplaceTool::title() const { return i18nc("@title", "Replace"); }

bool ReplaceTool::hasSelectedData()   const { return mByteArrayView->hasSelectedData(); }
QString ReplaceTool::charCodingName() const { return mByteArrayView->charCodingName(); }

void ReplaceTool::setTargetModel(AbstractModel* model)
{
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
        connect(mByteArrayView, &ByteArrayView::readOnlyChanged, this, &ReplaceTool::onReadOnlyChanged);
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void ReplaceTool::setUserQueryAgent(QObject* userQueryAgent)
{
    mUserQueryAgent = userQueryAgent;
}

void ReplaceTool::setSearchData(const QByteArray& searchData)
{
//     const bool oldIsApplyable = isApplyable();

    mSearchData = searchData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setReplaceData(const QByteArray& replaceData)
{
//     const bool oldIsApplyable = isApplyable();

    mReplaceData = replaceData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
//     const bool oldIsApplyable = isApplyable();

    mCaseSensitivity = caseSensitivity;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setDoPrompt(bool doPrompt)
{
    mDoPrompt = doPrompt;
}

void ReplaceTool::replace(FindDirection direction, bool fromCursor, bool inSelection)
{
    Okteta::Address startIndex;

    Okteta::Address replaceFirstIndex;
    Okteta::Address replaceLastIndex;
    bool doWrap;

    if (inSelection) {
        const Okteta::AddressRange selection = mByteArrayView->selection();
        if (!selection.isValid()) {
            mReplaceJob = nullptr;
            // nothing selected, so skip any search and finish now
            emit finished(false, 0);
            return;
        }

        replaceFirstIndex = selection.start();
        replaceLastIndex =  selection.end();
        startIndex = selection.start();
        doWrap = false;
        // TODO: support finding following selection direction
        direction = FindForward;
    } else {
        const Okteta::Address cursorPosition = mByteArrayView->cursorPosition();
        if (fromCursor && (cursorPosition != 0)) {
            replaceFirstIndex = cursorPosition;
            replaceLastIndex =  cursorPosition - 1;
        } else {
            replaceFirstIndex = 0;
            replaceLastIndex =  mByteArrayModel->size() - 1;
        }
        startIndex = (direction == FindForward) ? replaceFirstIndex : replaceLastIndex /*-mSearchData.size()*/;
        doWrap = (direction == FindForward) ? (replaceLastIndex < startIndex) : (startIndex < replaceFirstIndex);
    }

    mReplaceJob = new ReplaceJob(mByteArrayView, mByteArrayModel, mUserQueryAgent, this);
    mReplaceJob->setSearchData(mSearchData);
    mReplaceJob->setReplaceData(mReplaceData);
    mReplaceJob->setCaseSensitivity(mCaseSensitivity);
    mReplaceJob->setDoPrompt(mDoPrompt);
    mReplaceJob->setRange(replaceFirstIndex, replaceLastIndex, direction, startIndex, doWrap);
    connect(mReplaceJob, &ReplaceJob::finished, this, &ReplaceTool::onJobFinished);

    emit isApplyableChanged(isApplyable());

    mReplaceJob->start();
}

void ReplaceTool::onReadOnlyChanged(bool isReadOnly)
{
    Q_UNUSED(isReadOnly)

    // TODO: find out if isApplyable really changed, perhaps by caching the readonly state?
    emit isApplyableChanged(isApplyable());
}

void ReplaceTool::onJobFinished(bool previousFound, int noOfReplacements)
{
    delete mReplaceJob;
    mReplaceJob = nullptr;

    emit finished(previousFound, noOfReplacements);
    emit isApplyableChanged(isApplyable());
}

}
