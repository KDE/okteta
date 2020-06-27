/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchtool.hpp"

// controller
#include "searchuserqueryable.hpp"
#include "searchjob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QApplication>

namespace Kasten {

SearchTool::SearchTool()
{
    setObjectName(QStringLiteral("Search"));
}

SearchTool::~SearchTool() = default;

bool SearchTool::isApplyable() const
{
    return (mByteArrayView && mByteArrayModel);
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayView && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString SearchTool::title() const { return i18nc("@title", "Search"); }

bool SearchTool::hasSelectedData()   const { return mByteArrayView->hasSelectedData(); }
QString SearchTool::charCodingName() const { return mByteArrayView->charCodingName(); }

void SearchTool::setTargetModel(AbstractModel* model)
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
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                this, &SearchTool::charCodecChanged);
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SearchTool::setUserQueryAgent(If::SearchUserQueryable* userQueryAgent)
{
    mUserQueryAgent = userQueryAgent;
}

void SearchTool::setSearchData(const QByteArray& searchData)
{
//     const bool oldIsApplyable = isApplyable();

    mSearchData = searchData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
//     const bool oldIsApplyable = isApplyable();

    mCaseSensitivity = caseSensitivity;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::search(FindDirection direction, bool fromCursor, bool inSelection)
{
    mPreviousFound = false;

    if (inSelection) {
        const Okteta::AddressRange selection = mByteArrayView->selection();
        if (!selection.isValid()) {
            // nothing selected, so skip any search and finish now
            emit dataNotFound();
            return;
        }

        mSearchFirstIndex = selection.start();
        mSearchLastIndex =  selection.end();
    } else {
        const Okteta::Address cursorPosition = mByteArrayView->cursorPosition();
        if (fromCursor && (cursorPosition != 0)) {
            mSearchFirstIndex = cursorPosition;
            mSearchLastIndex =  cursorPosition - 1;
        } else {
            mSearchFirstIndex = 0;
            mSearchLastIndex =  mByteArrayModel->size() - 1;
        }
    }

    doSearch(direction);
}

void SearchTool::doSearch(FindDirection direction)
{
    // TODO: should start at last
    Okteta::Address startIndex = (direction == FindForward) ? mSearchFirstIndex : mSearchLastIndex /*-mSearchData.size()*/;
    bool wrapEnabled = (direction == FindForward) ? (mSearchLastIndex < startIndex) : (startIndex < mSearchFirstIndex);

    while (true) {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        Okteta::Address endIndex = wrapEnabled ?
                                   ((direction == FindForward) ? mByteArrayModel->size() - 1 : 0) :
                                   ((direction == FindForward) ? mSearchLastIndex : mSearchFirstIndex);

        SearchJob* searchJob =
            new SearchJob(mByteArrayModel, mSearchData, startIndex, endIndex, mCaseSensitivity, mByteArrayView->charCodingName());
        const Okteta::Address pos = searchJob->exec();

        QApplication::restoreOverrideCursor();

        if (pos != -1) {
            mPreviousFound = true;
            mByteArrayView->setSelection(pos, pos + mSearchData.size() - 1);
            break;
        }

        if (wrapEnabled) {
            const bool wrapping = mUserQueryAgent ? mUserQueryAgent->queryContinue(direction) : true;

            if (!wrapping) {
                break;
            }
            startIndex = (direction == FindForward) ? 0 : mByteArrayModel->size() - 1;
            wrapEnabled = false;
        } else {
            if (!mPreviousFound) {
                emit dataNotFound();
            }
            break;
        }
    }
    mByteArrayView->setFocus();
}

}
