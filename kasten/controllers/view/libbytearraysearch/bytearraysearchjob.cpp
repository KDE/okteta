/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysearchjob.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
// Qt
#include <QCoreApplication>

namespace Kasten {

ByteArraySearchJob::ByteArraySearchJob(const Okteta::AbstractByteArrayModel* model,
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

ByteArraySearchJob::~ByteArraySearchJob()
{
    delete mCharCodec;
}

Okteta::AddressRange ByteArraySearchJob::exec()
{
    start();
    // no own event loop here or mutex, we know that start is sync call currently
    return m_result;
}

void ByteArraySearchJob::start()
{
    // TODO: what kind of signal could a filter send?
    connect(mByteArrayModel, &Okteta::AbstractByteArrayModel::searchedBytes, this, &ByteArraySearchJob::onBytesSearched);

    const bool searchForward = (mStartIndex < mEndIndex);
    Okteta::Address resultIndex;
    if (searchForward) {
        resultIndex = (mCaseSensitivity == Qt::CaseSensitive) ?
                 mByteArrayModel->indexOf(mSearchData, mStartIndex, mEndIndex) :
                 mByteArrayModel->indexOfCaseInsensitive(mCharCodec, mSearchData, mStartIndex, mEndIndex);
    } else {
        const Okteta::Address lastFromIndex = mStartIndex - mSearchData.size() + 1;
        // Does not fit? Also prevent negative value for lastFromIndex,
        // as fromOffset arg of lastIndexOf() handles -1 differently and is undefined for other < 0 values.
        if (lastFromIndex < mEndIndex) {
            resultIndex = -1;
        } else {
            resultIndex = (mCaseSensitivity == Qt::CaseSensitive) ?
                 mByteArrayModel->lastIndexOf(mSearchData, lastFromIndex, mEndIndex) :
                 mByteArrayModel->lastIndexOfCaseInsensitive(mCharCodec, mSearchData, lastFromIndex, mEndIndex);
        }
    }
    m_result = (resultIndex != -1) ? Okteta::AddressRange::fromWidth(resultIndex, mSearchData.size()) : Okteta::AddressRange();
    emit finished(m_result);

    deleteLater(); // TODO: could be reused on next search
}

void ByteArraySearchJob::onBytesSearched()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
}

}

#include "moc_bytearraysearchjob.cpp"
