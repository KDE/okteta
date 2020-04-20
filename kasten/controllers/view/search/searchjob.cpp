/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

    Q_EMIT finished(m_result);

    deleteLater(); // TODO: could be reused on next search
}

void SearchJob::onBytesSearched()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
}

}
