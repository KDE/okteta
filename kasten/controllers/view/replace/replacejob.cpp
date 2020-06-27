/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
        qRegisterMetaType<Kasten::ReplaceBehaviour>();
        connect(m_userQueryAgent, SIGNAL(queryContinueFinished(bool)),
                this, SLOT(handleContinueFinished(bool)), Qt::QueuedConnection);
        connect(m_userQueryAgent, SIGNAL(queryReplaceCurrentFinished(Kasten::ReplaceBehaviour)),
                this, SLOT(handleReplaceCurrentFinished(Kasten::ReplaceBehaviour)), Qt::QueuedConnection);
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

void ReplaceJob::setRange(Okteta::Address replaceRangeStartIndex, Okteta::Address replaceRangeEndIndex,
                          FindDirection direction)
{
    m_replaceRangeStartIndex = replaceRangeStartIndex;
    m_replaceRangeEndIndex = replaceRangeEndIndex;
    
    m_direction = direction;

    m_currentIndex = (m_direction == FindForward) ? m_replaceRangeStartIndex : m_replaceRangeEndIndex;

    m_doWrap = (m_replaceRangeEndIndex < m_replaceRangeStartIndex);
}

void ReplaceJob::start()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_noOfReplacements = 0;
    m_currentReplaceRangeStartIndex = (!m_doWrap || (m_direction == FindForward)) ? m_replaceRangeStartIndex : 0;
    m_currentReplaceRangeEndIndex = (!m_doWrap || (m_direction == FindBackward)) ? m_replaceRangeEndIndex : m_byteArrayModel->size() - 1;

    searchNextPosition();
}

void ReplaceJob::searchNextPosition()
{
    const bool isForward = (m_direction == FindForward);

    if ((isForward && m_currentIndex > m_currentReplaceRangeEndIndex - m_searchData.size() + 1) ||
        (!isForward && (m_currentIndex < m_currentReplaceRangeStartIndex + m_searchData.size() - 1))) {
        handleEndReached();
        return;
    }

    const Okteta::Address endIndex = isForward ? m_currentReplaceRangeEndIndex : m_currentReplaceRangeStartIndex;

    auto* searchJob = new SearchJob(m_byteArrayModel, m_searchData, m_currentIndex, endIndex,
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
    m_currentIndex = pos;

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
            ++m_currentIndex;
        } else {
            --m_currentIndex;
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
    const Okteta::Size inserted = m_byteArrayModel->replace(m_currentIndex, m_searchData.size(),
                                                            reinterpret_cast<const Okteta::Byte*>(m_replaceData.constData()),
                                                            m_replaceData.size());
    const Okteta::Size sizeDiff = inserted - m_searchData.size();

    // TODO: cursors being automatically updated by the model would be nice to have here
    if (m_replaceRangeEndIndex >= m_currentIndex) {
        m_replaceRangeEndIndex += sizeDiff;
        if (m_replaceRangeEndIndex < 0) {
            m_replaceRangeEndIndex = 0;
        }
    }
    if (m_currentReplaceRangeEndIndex >= m_currentIndex) {
        m_currentReplaceRangeEndIndex += sizeDiff;
        if (m_currentReplaceRangeEndIndex < 0) {
            m_currentReplaceRangeEndIndex = 0;
        }
    }
    if (m_replaceRangeStartIndex > m_currentIndex) {
        m_replaceRangeStartIndex += sizeDiff;
        if (m_replaceRangeStartIndex < 0) {
            m_replaceRangeStartIndex = 0;
        }
    }
    if (m_currentReplaceRangeStartIndex > m_currentIndex) {
        m_currentReplaceRangeStartIndex += sizeDiff;
        if (m_currentReplaceRangeStartIndex < 0) {
            m_currentReplaceRangeStartIndex = 0;
        }
    }

    if (m_direction == FindForward) {
        m_currentIndex += inserted;
    } else {
        --m_currentIndex;
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
    m_currentReplaceRangeStartIndex = (m_direction == FindForward) ? 0 : m_replaceRangeStartIndex;
    m_currentReplaceRangeEndIndex = (m_direction == FindForward) ? m_replaceRangeEndIndex : m_byteArrayModel->size() - 1;
    m_currentIndex = (m_direction == FindForward) ? 0 : m_byteArrayModel->size() - 1;
    m_doWrap = false;

    searchNextPosition();
}

void ReplaceJob::finish()
{
    QApplication::restoreOverrideCursor();
    emit finished(m_previousFound, m_noOfReplacements);
}

}
