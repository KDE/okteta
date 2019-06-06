/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "replacejob.hpp"

// search controller
#include "../search/searchjob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QApplication>

namespace Kasten {

ReplaceJob::ReplaceJob(ByteArrayView* byteArrayView, Okteta::AbstractByteArrayModel* byteArrayModel,
                        QObject* userQueryAgent,
                        QObject* parent)
    : QObject(parent)
    , m_userQueryAgent(userQueryAgent)
    , m_byteArrayView(byteArrayView)
    , m_byteArrayModel(byteArrayModel)
{
    auto* replaceUserQueryable = qobject_cast<If::ReplaceUserQueryable*>(m_userQueryAgent);
    if (replaceUserQueryable) {
        connect(m_userQueryAgent, SIGNAL(queryContinueFinished(bool)),
                this, SLOT(handleContinueFinished(bool)), Qt::QueuedConnection);
        connect(m_userQueryAgent, SIGNAL(queryReplaceCurrentFinished(ReplaceBehaviour)),
                this, SLOT(handleReplaceCurrentFinished(ReplaceBehaviour)), Qt::QueuedConnection);
    } else {
        m_userQueryAgent = nullptr;
    }
}

ReplaceJob::~ReplaceJob() = default;

void ReplaceJob::setSearchData(const QByteArray& searchData)
{
    m_searchData = searchData;
}

void ReplaceJob::setReplaceData(const QByteArray& replaceData)
{
    m_replaceData = replaceData;
}

void ReplaceJob::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
    m_caseSensitivity = caseSensitivity;
}

void ReplaceJob::setDoPrompt(bool doPrompt)
{
    m_doPrompt = doPrompt;
}

void ReplaceJob::setRange(Okteta::Address replaceFirstIndex, Okteta::Address replaceLastIndex,
                          FindDirection direction, Okteta::Address startIndex, bool doWrap)
{
    m_replaceFirstIndex = replaceFirstIndex;
    m_replaceLastIndex = replaceLastIndex;
    
    m_direction = direction;
    m_startIndex = startIndex;

    m_doWrap = doWrap;
}

void ReplaceJob::start()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_noOfReplacements = 0;

    searchNextPosition();
}

void ReplaceJob::searchNextPosition()
{
    const bool isForward = (m_direction == FindForward);

    if ((isForward && m_startIndex > m_byteArrayModel->size() - 1) ||
        (!isForward && m_startIndex < 0)) {
        handleEndReached();
        return;
    }

    const Okteta::Address endIndex = m_doWrap ?
        (isForward ? m_byteArrayModel->size() - 1 : 0) :
        (isForward ? m_replaceLastIndex : m_replaceFirstIndex);

    auto* searchJob = new SearchJob(m_byteArrayModel, m_searchData, m_startIndex, endIndex,
                                    m_caseSensitivity, m_byteArrayView->charCodingName());
    // Qt::QueuedConnection to ensure passing the event loop, so we do not recursively fill the callstack
    // as any async calls (query user, search) could fire signal while being invoked
    // TODO: optimize for non-user-querying with loop variant
    connect(searchJob, &SearchJob::finished, this, &ReplaceJob::handleSearchResult, Qt::QueuedConnection);
    searchJob->start();
}

void ReplaceJob::handleSearchResult(Okteta::Address pos)
{
    if (pos == -1) {
        handleEndReached();
        return;
    }

    m_previousFound = true;
    m_startIndex = pos;

    if (m_doPrompt && m_userQueryAgent) {
        QApplication::restoreOverrideCursor();

        m_byteArrayView->setSelection(pos, pos + m_searchData.size() - 1);

        qobject_cast<If::ReplaceUserQueryable*>(m_userQueryAgent)->queryReplaceCurrent();
    } else {
        replaceCurrent();
    }
}

void ReplaceJob::handleReplaceCurrentFinished(ReplaceBehaviour replaceBehaviour)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_byteArrayView->selectAllData(false);

    bool currentToBeReplaced;
    bool isCancelled;

    switch (replaceBehaviour)
    {
    case ReplaceAll:
        m_doPrompt = false;
        currentToBeReplaced = true;
        isCancelled = false;
        break;
    case ReplaceCurrent:
        currentToBeReplaced = true;
        isCancelled = false;
        break;
    case SkipCurrent:
        if (m_direction == FindForward) {
            ++m_startIndex;
        } else {
            --m_startIndex;
        }
        currentToBeReplaced = false;
        isCancelled = false;
        break;
    case CancelReplacing:
    default:
        currentToBeReplaced = false;
        isCancelled = true;
        m_doWrap = false;
    }

    if (isCancelled) {
        finish();
    } else if (currentToBeReplaced) {
        replaceCurrent();
    } else {
        searchNextPosition();
    }
}

void ReplaceJob::replaceCurrent()
{
    ++m_noOfReplacements;
    const Okteta::Size inserted = m_byteArrayModel->replace(m_startIndex, m_searchData.size(),
                                                            reinterpret_cast<const Okteta::Byte*>(m_replaceData.constData()),
                                                            m_replaceData.size());
    if (m_direction == FindForward) {
        m_startIndex += inserted;
    } else {
        m_startIndex -= inserted;
    }

    searchNextPosition();
}

void ReplaceJob::handleEndReached()
{
    // reached end
    if (m_doWrap) {
        if (m_userQueryAgent) {
            QApplication::restoreOverrideCursor();
            qobject_cast<If::ReplaceUserQueryable*>(m_userQueryAgent)->queryContinue(m_direction, m_noOfReplacements);
            // TODO: resetting the count as expected due to current already reported in query
            // ruins the generic meaning of the value passed finished signal, perhaps add separate totalNo?
            m_noOfReplacements = 0;
        } else {
            wrapAndSearchNextPosition();
        }
    } else {
        finish();
    }
}

void ReplaceJob::handleContinueFinished(bool result)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (result) {
        wrapAndSearchNextPosition();
    } else {
        finish();
    }
}

void ReplaceJob::wrapAndSearchNextPosition()
{
    m_startIndex = (m_direction == FindForward) ? 0 : m_byteArrayModel->size() - 1;
    m_doWrap = false;

    searchNextPosition();
}

void ReplaceJob::finish()
{
    QApplication::restoreOverrideCursor();
    emit finished(m_previousFound, m_noOfReplacements);
}

}
