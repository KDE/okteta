/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchjob.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
// Qt
#include <QCoreApplication>

namespace Kasten {

SearchJob::SearchJob(const Okteta::AbstractByteArrayModel* model,
                     const QByteArray& searchData,
                     Okteta::Address startIndex, Okteta::Address endIndex,
                     Qt::CaseSensitivity caseSensitivity, const QString& charCodecName)
    : mByteArrayModel(model)
    , mSearchData(searchData)
    , mStartIndex(startIndex)
    , mEndIndex(endIndex)
    , mCaseSensitivity(caseSensitivity)
    , mCharCodec(Okteta::CharCodec::createCodec(charCodecName))
{
}

SearchJob::~SearchJob()
{
    delete mCharCodec;
}

Okteta::Address SearchJob::exec()
{
    start();
    // no own event loop here or mutex, we know that start is sync call currently
    return m_result;
}

void SearchJob::start()
{
    // TODO: what kind of signal could a filter send?
    connect(mByteArrayModel, &Okteta::AbstractByteArrayModel::searchedBytes, this, &SearchJob::onBytesSearched);

    const bool searchForward = (mStartIndex < mEndIndex);
    if (searchForward) {
        m_result = (mCaseSensitivity == Qt::CaseSensitive) ?
                 mByteArrayModel->indexOf(mSearchData, mStartIndex, mEndIndex) :
                 mByteArrayModel->indexOfCaseInsensitive(mCharCodec, mSearchData, mStartIndex, mEndIndex);
    } else {
        const Okteta::Address lastFromIndex = mStartIndex - mSearchData.size() + 1;
        m_result = (mCaseSensitivity == Qt::CaseSensitive) ?
                 mByteArrayModel->lastIndexOf(mSearchData, lastFromIndex, mEndIndex) :
                 mByteArrayModel->lastIndexOfCaseInsensitive(mCharCodec, mSearchData, lastFromIndex, mEndIndex);
    }

    emit finished(m_result);

    deleteLater(); // TODO: could be reused on next search
}

void SearchJob::onBytesSearched()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
}

}
